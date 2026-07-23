efi:
	rm -f ./hwlist.img
	rm -rf ./img/
	rm -rf ./mnt/

	mkdir -p img/EFI/BOOT
	x86_64-w64-mingw32-gcc \
		-mno-red-zone \
		-ffreestanding \
		-nostdlib \
		-e efi_main \
		-Wl,-subsystem,10 \
		-o img/EFI/BOOT/BOOTX64.efi \
		src/*.c

	touch hwlist.img
	mkdir -p ./mnt/
	dd if=/dev/zero of=hwlist.img bs=1M count=64
	mkfs.fat -F 32 hwlist.img
	sudo mount -o loop hwlist.img ./mnt/
	sudo cp -r ./img/* ./mnt/
	sudo umount ./mnt/
	rm -rf ./mnt