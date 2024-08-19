
dd if=/dev/zero of=fat32.img bs=1M count=32
mkfs.fat -F 32 fat32.img
mv fat32.img resources/

dd if=/dev/zero of=fat12.img bs=1M count=12
mkfs.fat -F 12 fat12.img
mv fat12.img resources/

dd if=/dev/zero of=fat16.img bs=1M count=16
mkfs.fat -F 16 fat16.img
mv fat16.img resources/


dd if=/dev/zero of=ext3.img bs=1M count=8
mkfs.ext3 ext3.img
mv ext3.img resources/

dd if=/dev/zero of=ext4.img bs=1M count=8
mkfs.ext4 ext4.img
mv ext4.img resources/

dd if=/dev/zero of=exfat.img bs=1M count=8
mkfs.exfat exfat.img
mv exfat.img resources/

dd if=/dev/zero of=ntfs.img bs=1M count=8
sudo losetup /dev/loop0 ntfs.img
sudo mkfs.ntfs /dev/loop0
sudo losetup -d /dev/loop0
mv ntfs.img resources/ntfs.img

xorriso -as mkisofs -J -R -o resources/iso9660.iso test_iso

dd if=/dev/zero of=vfat.img bs=1M count=8
mkfs.vfat vfat.img
mv vfat.img resources/vfat.img

dd if=/dev/zero of=btrfs.img bs=32K count=3488
mkfs.btrfs btrfs.img
mv btrfs.img resources/btrfs.img


