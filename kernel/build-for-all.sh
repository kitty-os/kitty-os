set -e

make -j12 ARCH=x86_64
rm -rf obj
clear
echo "=== DONE COMPILING FOR x86_64 ==="
mv kernel.elf x86-64-kernel.elf

make -j12 ARCH=aarch64
rm -rf obj
clear
echo "=== DONE COMPILING FOR aarch64 ==="
mv kernel.elf aarch64-kernel.elf

make -j12 ARCH=riscv64
rm -rf obj
clear
echo "=== DONE COMPILING FOR riscv64 ==="
mv kernel.elf riscv64-kernel.elf

make -j12 ARCH=loongarch64
rm -rf obj
clear
echo "=== DONE COMPILING FOR loongarch64 ==="
mv kernel.elf loongarch64-kernel.elf

clear

echo "=== DONE COMPILING FOR ALL ARCHITECTURES =="