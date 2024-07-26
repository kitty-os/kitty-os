DRIVER_NAME="HelloWorld"
WORKING_DIR="src"
BUILD_DIR="build"
OBJECTS_DIR="build/obj"
CC="x86_64-w64-mingw32-gcc"
CFLAGS="-ffreestanding -nostdlib"
LD="x86_64-w64-mingw32-ld"
LDFLAGS="-ffreestanding -nostdlib --shared -o $BUILD_DIR/$DRIVER_NAME.dll -e DriverEntry --output-def mylib.def $WORKING_DIR/$DRIVER_NAME.def"

set -x

mkdir -p $BUILD_DIR
mkdir -p $OBJECTS_DIR

$CC $WORKING_DIR/Main.cpp $CFLAGS -c -o $OBJECTS_DIR/Main.cpp.obj
$LD $LDFLAGS $OBJECTS_DIR/Main.cpp.obj