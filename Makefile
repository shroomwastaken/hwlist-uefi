efi:
	rm -f ./hwlist.img
	rm -rf ./iso/
	rm -rf ./mnt/

	mkdir -p iso/EFI/BOOT
	x86_64-w64-mingw32-gcc \
		-mno-red-zone \
		-ffreestanding \
		-nostdlib \
		-e efi_main \
		-Wl,-subsystem,10 \
		-o iso/EFI/BOOT/BOOTX64.efi \
		src/efi.c

	touch hwlist.img
	mkdir -p ./mnt/
	dd if=/dev/zero of=hwlist.img bs=1M count=64
	mkfs.fat -F 32 hwlist.img
	sudo mount -o loop hwlist.img ./mnt/
	sudo cp -r ./iso/* ./mnt/
	sudo umount ./mnt/
	rm -rf ./mnt