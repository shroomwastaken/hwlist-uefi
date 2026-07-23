# hwlist-uefi

bootable uefi program that lists information about device hardware

## building
only buildable on linux

you'll need the `x86_64-w64-mingw32-gcc` cross compiler and `mkfs.fat`.
in my case (Arch Linux) these are in the `mingw-w64-gcc` and `dosfstools` packages respectively.

running `make` will output `img/EFI/BOOT/BOOTX64.EFI` (the application itself) and `hwlist.img` (drive image).

`make` will ask for sudo password to mount the image and copy the EFI file into it.

## running
run with `qemu-system-x86_64` + ovmf bios.

**this is an experiment. running on actual hardware is probably a bad idea at the moment. do so at your own risk.**