#!/usr/bin/bash

qemu-system-aarch64 -M virt -cpu cortex-a72 -device ramfb -device qemu-xhci -device usb-kbd -device usb-mouse -cdrom aarch64-kitty.iso -drive if=pflash,unit=0,format=raw,file=ovmf/ovmf-code-aarch64.fd,readonly=on -drive if=pflash,unit=1,format=raw,file=ovmf/ovmf-vars-aarch64.fd