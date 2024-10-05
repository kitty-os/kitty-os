#!/usr/bin/bash

qemu-system-x86_64 -M q35 -m 4G -smp 4 -cdrom x86_64-kitty.iso
