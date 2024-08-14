bash create_test_image.sh
cp testimg.img resources/

cd drivers/HelloWorld/
./Build.sh
cd ../../
mkdir -p compiled_drivers
cp drivers/HelloWorld/Build/HelloWorld.sys compiled_drivers/HelloWorld.sys

set -e

# for some reason makefile isnt doing table.cpp for us.
cd kernel/src/Hal/Int/
python3 create_table.py
cd ../../../../

make run -j12
