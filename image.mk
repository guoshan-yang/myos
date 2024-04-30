
$(BUILD)/hd.img: ${BUILD}/boot/boot.bin ${BUILD}/boot/loader.bin \
		$(BUILD)/system.bin \
    	$(BUILD)/system.map \
    	$(SRC)/config/master.fdisk \
    	$(BUILD)/builtin/hello.out \

	$(shell rm -rf $(BUILD)/$(HD_IMG_NAME))
	yes | bximage -q -hd=16 -func=create -sectsize=512 -imgmode=flat $(BUILD)/$(HD_IMG_NAME)
	dd if=${BUILD}/boot/boot.bin of=$(BUILD)/$(HD_IMG_NAME) bs=512 seek=0 count=1 conv=notrunc
	dd if=${BUILD}/boot/loader.bin of=$(BUILD)/$(HD_IMG_NAME) bs=512 seek=1 count=2 conv=notrunc
	dd if=$(BUILD)/system.bin of=$(BUILD)/$(HD_IMG_NAME) bs=512 count=200 seek=3 conv=notrunc
	# 对硬盘进行分区
	sfdisk $@ < $(SRC)/config/master.fdisk

	# 挂载设备
	echo "100200" | sudo -S losetup /dev/loop100 --partscan $@

	# 创建 minux 文件系统
	echo "100200" | sudo -S mkfs.minix -1 -n 14 /dev/loop100p1

	# 挂载文件系统
	echo "100200" | sudo -S mount /dev/loop100p1 /mnt

	# 切换所有者
	echo "100200" | sudo -S chown ${USER} /mnt

	# 创建目录
	mkdir -p /mnt/dev
	mkdir -p /mnt/mnt

	# 创建文件
	echo "hello onix!!!, from root direcotry file..." > /mnt/hello.txt

	# 拷贝程序
	cp $(BUILD)/builtin/hello.out /mnt/hello.out

	# 卸载文件系统
	echo "100200" | sudo -S umount /mnt

	# 卸载设备
	echo "100200" | sudo -S losetup -d /dev/loop100


$(BUILD)/slave.img: $(SRC)/config/slave.fdisk
	yes | bximage -q -hd=32 -func=create -sectsize=512 -imgmode=flat $(BUILD)/$(SLAVE_IMG_NAME)

	# 执行硬盘分区
	sfdisk $@ < $(SRC)/config/slave.fdisk

	# 挂载设备
	sudo losetup /dev/loop100 --partscan $@

	# 创建 minux 文件系统-1版本是1 -n文件名长度
	sudo mkfs.minix -1 -n 14 /dev/loop100p1

	# 挂载文件系统
	sudo mount /dev/loop100p1 /mnt

	# 切换所有者
	sudo chown ${USER} /mnt

	# 创建文件
	echo "slave root direcotry file..." > /mnt/hello.txt

	# 卸载文件系统
	sudo umount /mnt

	# 卸载设备
	sudo losetup -d /dev/loop100

.PHONY: mount0
mount0: $(BUILD)/hd.img
	echo "100200" | sudo -S losetup /dev/loop100 --partscan $<
	echo "100200" | sudo -S mount /dev/loop100p1 /mnt
	echo "100200" | sudo -S chown ${USER} /mnt

.PHONY: umount0
umount0: /dev/loop100
	-echo "100200" | sudo -S umount /mnt
	-echo "100200" | sudo -S losetup -d $<

IMAGES:= $(BUILD)/hd.img \
		$(BUILD)/slave.img

image: $(IMAGES)