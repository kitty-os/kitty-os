qemu-system-loongarch64 -M virt -cpu la464 -device ramfb -device qemu-xhci -device usb-kbd -device usb-mouse -drive if=pflash,unit=0,format=raw,file=ovmf/ovmf-code-loongarch64.fd,readonly=on -drive if=pflash,unit=1,format=raw,file=ovmf/ovmf-vars-loongarch64.fd -cdrom loongarch64-kitty.iso