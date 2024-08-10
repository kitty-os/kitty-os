import re

# Input and output file paths
PCI_IDS_FILE = 'pci.ids'
OUTPUT_HEADER_FILE = 'pci_db.hpp'
OUTPUT_CPP_FILE = 'pci_db.cpp'

# Function to escape C++ string literals
def escape_cpp_string(s):
    return s.replace('\\', '\\\\').replace('"', '\\"')

# Function to parse the pci.ids file
def parse_pci_ids(file_path):
    with open(file_path, 'r') as f:
        lines = f.readlines()

    vendor_device_map = {}
    current_vendor_id = None
    current_device_id = None

    for line in lines:
        # Skip comments and empty lines
        if line.startswith('#') or not line.strip():
            continue

        # Match vendor lines
        vendor_match = re.match(r'^([0-9a-fA-F]{4})\s+(.+)$', line)
        if vendor_match:
            current_vendor_id = int(vendor_match.group(1), 16)
            vendor_name = vendor_match.group(2).strip()
            vendor_device_map[current_vendor_id] = {
                'name': vendor_name,
                'devices': {}
            }
            current_device_id = None
            continue

        # Match device lines
        device_match = re.match(r'^\t([0-9a-fA-F]{4})\s+(.+)$', line)
        if device_match and current_vendor_id is not None:
            current_device_id = int(device_match.group(1), 16)
            device_name = device_match.group(2).strip()
            vendor_device_map[current_vendor_id]['devices'][current_device_id] = {
                'name': device_name,
                'subsystems': {}
            }
            continue

        # Match subsystem lines
        subsystem_match = re.match(r'^\t\t([0-9a-fA-F]{4})\s+([0-9a-fA-F]{4})\s+(.+)$', line)
        if subsystem_match and current_vendor_id is not None and current_device_id is not None:
            subvendor_id = int(subsystem_match.group(1), 16)
            subdevice_id = int(subsystem_match.group(2), 16)
            subsystem_name = subsystem_match.group(3).strip()
            vendor_device_map[current_vendor_id]['devices'][current_device_id]['subsystems'][
                (subvendor_id, subdevice_id)
            ] = subsystem_name

    return vendor_device_map

# Function to generate the header file
def generate_header_file(vendor_device_map):
    total_entries = 0

    for vendor_data in vendor_device_map.values():
        for device_data in vendor_data['devices'].values():
            # Count each device
            total_entries += 1
            # Count each subsystem as an additional entry
            total_entries += len(device_data['subsystems'])

    with open(OUTPUT_HEADER_FILE, 'w') as f:
        f.write('#pragma once\n\n')
        f.write('#include <cstdint>\n\n')
        f.write('#include <Hal/Pci/pci.hpp>\n\n')
        f.write(f'extern PCIDeviceDatabaseEntry pci_device_database[{total_entries}];\n')
        f.write(f'constexpr size_t pci_device_database_size = {total_entries};\n')

# Function to generate the cpp file
def generate_cpp_file(vendor_device_map):
    with open(OUTPUT_CPP_FILE, 'w') as f:
        f.write('#include "pci.hpp"\n\n')
        f.write('PCIDeviceDatabaseEntry pci_device_database[] = {\n')

        for vendor_id, vendor_data in vendor_device_map.items():
            vendor_name = escape_cpp_string(vendor_data["name"])
            for device_id, device_data in vendor_data['devices'].items():
                device_name = escape_cpp_string(device_data["name"])

                # If there are subsystems, add them
                if device_data['subsystems']:
                    for (subvendor_id, subdevice_id), subsystem_name in device_data['subsystems'].items():
                        subsystem_name = escape_cpp_string(subsystem_name)
                        f.write(f'    {{"{vendor_name}", "{device_name}", 0x{vendor_id:04X}, 0x{device_id:04X}, '
                                f'0x{subvendor_id:04X}, 0x{subdevice_id:04X}, "{subsystem_name}"}},\n')
                else:
                    # If no subsystems, add with null subsystem fields
                    f.write(f'    {{"{vendor_name}", "{device_name}", 0x{vendor_id:04X}, 0x{device_id:04X}, '
                            f'0, 0, nullptr}},\n')

        f.write('};\n')

# Main script execution
if __name__ == '__main__':
    vendor_device_map = parse_pci_ids(PCI_IDS_FILE)
    generate_header_file(vendor_device_map)
    generate_cpp_file(vendor_device_map)
