BUILD:=./build
SRC:=.
HD_IMG_NAME:= "hd.img"
SLAVE_IMG_NAME:= "slave.img"
MULTIBOOT2:=0x10000
ENTRYPOINT:=$(shell python -c "print(f'0x{$(MULTIBOOT2) + 64:x}')")

CFLAGS:= -m32 # 32 位的程序
CFLAGS+= -fno-builtin	# 不需要 gcc 内置函数
CFLAGS+= -nostdinc		# 不需要标准头文件
CFLAGS+= -fno-pic		# 不需要位置无关的代码  position independent code
CFLAGS+= -fno-pie		# 不需要位置无关的可执行程序 position independent executable
CFLAGS+= -nostdlib		# 不需要标准库
CFLAGS+= -fno-stack-protector	# 不需要栈保护
CFLAGS:=$(strip ${CFLAGS})

DEBUG:= -g

${BUILD}/boot/%.bin: boot/%.asm
	$(shell mkdir -p ${BUILD}/boot)
	nasm -f bin $< -o $@

${BUILD}/%.o: %.asm
	$(shell mkdir -p ${BUILD}/kernel)
	nasm -f elf32 $(DEBUG) $< -o $@

$(BUILD)/%.o: %.c
	$(shell mkdir -p $(dir $@))
	gcc $(CFLAGS) $(DEBUG) -c $< -o $@

LDFLAGS:= -m elf_i386 \
		-static \
		-Ttext $(ENTRYPOINT)\
		--section-start=.multiboot2=$(MULTIBOOT2)
LDFLAGS:=$(strip ${LDFLAGS})

$(BUILD)/kernel/kernel.bin: $(BUILD)/kernel/entry_kernel.o \
	$(BUILD)/kernel/main.o \
	$(BUILD)/kernel/io.o \
	$(BUILD)/kernel/console.o \
	$(BUILD)/kernel/printk.o \
	$(BUILD)/kernel/vsprintf.o \
	$(BUILD)/kernel/debug.o \
	$(BUILD)/kernel/gdt.o \
	$(BUILD)/kernel/task.o \
	$(BUILD)/kernel/schedule.o \
	$(BUILD)/kernel/idt.o \
	$(BUILD)/kernel/interrupt_handler.o \
	$(BUILD)/kernel/assert.o \
	$(BUILD)/kernel/clock.o \
	$(BUILD)/kernel/time.o \
	$(BUILD)/kernel/rtc.o \
	$(BUILD)/kernel/memory.o \
	$(BUILD)/kernel/gate.o \
	$(BUILD)/kernel/thread.o \
	$(BUILD)/kernel/mutex.o \
	$(BUILD)/kernel/keyboard.o \
	$(BUILD)/kernel/arena.o \
	$(BUILD)/kernel/ide.o \
	$(BUILD)/kernel/device.o \
	$(BUILD)/kernel/buffer.o \
	$(BUILD)/fs/super.o \
	$(BUILD)/fs/bmap.o \
	$(BUILD)/lib/bitmap.o \
	$(BUILD)/lib/string.o \
	$(BUILD)/lib/stdlib.o \
	$(BUILD)/lib/syscall.o \
	$(BUILD)/lib/list.o \
	$(BUILD)/lib/printf.o \
	$(BUILD)/lib/fifo.o \

	$(shell mkdir -p $(dir $@))
	ld ${LDFLAGS} $^ -o $@

$(BUILD)/system.bin: $(BUILD)/kernel/kernel.bin
	objcopy -O binary $< $@

$(BUILD)/system.map: $(BUILD)/kernel/kernel.bin
	nm $< | sort > $@

include image.mk

$(BUILD)/kernel.iso : $(BUILD)/kernel/kernel.bin $(SRC)/config/grub.cfg

# 检测内核文件是否合法
	grub-file --is-x86-multiboot2 $<
# 创建 iso 目录
	mkdir -p $(BUILD)/iso/boot/grub
# 拷贝内核文件
	cp $< $(BUILD)/iso/boot
# 拷贝 grub 配置文件
	cp $(SRC)/config/grub.cfg $(BUILD)/iso/boot/grub
# 生成 iso 文件
	grub-mkrescue -o $@ $(BUILD)/iso

test: ${BUILD}/kernel/entry_kernel.o

clean:
	$(shell rm -rf ${BUILD})

bochs: $(IMAGES)
	bochs -q -f ./bochs/bochsrc -unlock

bochs-grub: $(BUILD)/kernel.iso
	bochs -q -f ./bochs/bochsrc.grub -unlock

QEMU:= qemu-system-i386 \
	-m 32M \
	-boot c \
	-rtc base=localtime \

QEMU_DISK:=-boot c \
	-drive file=$(BUILD)/hd.img,if=ide,index=0,media=disk,format=raw \
	-drive file=$(BUILD)/slave.img,if=ide,index=1,media=disk,format=raw \

QEMU_CDROM:=-boot d \
	-drive file=$(BUILD)/kernel.iso,media=cdrom \

QEMU_DEBUG:= -s -S

qemu-gdb: $(IMAGES)
	$(QEMU) $(QEMU_DISK) $(QEMU_DEBUG)

qemu: $(IMAGES)
	$(QEMU) $(QEMU_DISK)

qemu-grub: $(BUILD)/kernel.iso
	$(QEMU) $(QEMU_CDROM)

$(BUILD)/hd.vmdk: $(BUILD)/hd.img
	qemu-img convert -pO vmdk $< $@

vmdk: $(BUILD)/hd.vmdk

