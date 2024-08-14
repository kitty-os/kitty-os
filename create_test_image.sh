dd if=/dev/zero of=testimg.img bs=1M count=33

mkfs.fat -F 32 testimg.img

mcopy -i testimg.img testfile.txt ::

mdir -i testimg.img ::
