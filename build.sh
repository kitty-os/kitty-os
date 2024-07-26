cd drivers/HelloWorld/
./Build.sh
cd ../../
mkdir -p compiled_drivers
cp drivers/HelloWorld/Build/HelloWorld.dll compiled_drivers/HelloWorld.sys
make run -j12
