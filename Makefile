BUILD:=./build
HD_IMG_NAME:= "hd.img"
ENTRYPOINT:=0x10000

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
	$(BUILD)/lib/string.o \

	$(shell mkdir -p $(dir $@))
	ld -m elf_i386 -static $^ -o $@ -Ttext $(ENTRYPOINT)

$(BUILD)/system.bin: $(BUILD)/kernel/kernel.bin
	objcopy -O binary $< $@

$(BUILD)/system.map: $(BUILD)/kernel/kernel.bin
	nm $< | sort > $@

$(BUILD)/hd.img: ${BUILD}/boot/boot.bin ${BUILD}/boot/loader.bin \
		$(BUILD)/system.bin \
    	$(BUILD)/system.map \

	$(shell rm -rf $(BUILD)/$(HD_IMG_NAME))
	bximage -q -hd=16 -func=create -sectsize=512 -imgmode=flat $(BUILD)/$(HD_IMG_NAME)
	dd if=${BUILD}/boot/boot.bin of=$(BUILD)/$(HD_IMG_NAME) bs=512 seek=0 count=1 conv=notrunc
	dd if=${BUILD}/boot/loader.bin of=$(BUILD)/$(HD_IMG_NAME) bs=512 seek=1 count=2 conv=notrunc
	dd if=$(BUILD)/system.bin of=$(BUILD)/$(HD_IMG_NAME) bs=512 count=200 seek=3 conv=notrunc

test: ${BUILD}/kernel/entry_kernel.o

clean:
	$(shell rm -rf ${BUILD})

bochs: $(BUILD)/hd.img
	bochs -q -f bochsrc

qemu-gdb: $(BUILD)/hd.img
	qemu-system-i386 \
	-s -S \
	-m 32M \
	-boot c \
	-hda ./build/hd.img

qemu: $(BUILD)/hd.img
	qemu-system-i386 \
	-m 32M \
	-boot c \
	-hda ./build/hd.img

$(BUILD)/hd.vmdk: $(BUILD)/hd.img
	qemu-img convert -pO vmdk $< $@

vmdk: $(BUILD)/hd.vmdk

