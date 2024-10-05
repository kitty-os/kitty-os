#!/usr/bin/bash

qemu-system-riscv64 -M virt -cpu rv64 -device ramfb -device qemu-xhci -device usb-kbd -device usb-mouse -drive if=pflash,unit=0,format=raw,file=ovmf/ovmf-code-riscv64.fd,readonly=on -drive if=pflash,unit=1,format=raw,file=ovmf/ovmf-vars-riscv64.fd -cdrom riscv64-kitty.iso