set -e

cd kernel
./build-for-all.sh
cd ../isogen
python3 geniso.py
cd ..
cp isogen/*.iso ./
