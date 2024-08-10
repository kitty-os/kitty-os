#!/usr/bin/bash

git clone --depth=1 https://github.com/pciutils/pciids pci_ids
cp pci_ids/pci.ids ./pci.ids

python3 generate_database.py

echo "Done!"
