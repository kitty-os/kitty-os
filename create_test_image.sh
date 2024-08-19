set -e

cp -r test_files/* test_iso/

dd if=/dev/zero of=ext2.img bs=1M count=8
mkfs.ext2 ext2.img
mv ext2.img resources/
