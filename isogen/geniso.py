import json
import os
import subprocess

def generate_lim_config_and_iso(json_file, output_conf_file, image_name, target_arch):
    # Load the JSON data from the file
    try:
        with open(json_file, 'r') as f:
            config_data = json.load(f)
    except (FileNotFoundError, json.JSONDecodeError) as e:
        print(f"Error reading JSON file: {e}")
        return

    print("Loaded configuration data:", config_data)  # Debugging line

    try:
        # Start building the configuration string
        config_lines = []

        # Add global timeout option
        config_lines.append(f"timeout: {config_data.get('timeout', '0')}")
        config_lines.append("")  # Blank line for separation

        # Process the specified architecture if it exists
        arch_data = config_data.get("kernel.files", {}).get(target_arch)

        print("Architecture data for target:", arch_data)  # Debugging line

        if isinstance(arch_data, dict):  # Check if arch_data is a dictionary
            os_name = arch_data.get('os-name', target_arch)
            config_lines.append(f"/{os_name} Kernel ({target_arch})")
            # Add 'boot():' prefix and remove 'resources/' from kernel path
            kernel_path = arch_data.get('kernel.path', '').replace('resources/', '')
            config_lines.append(f"    kernel_path: boot():/{kernel_path}")
            config_lines.append(f"    protocol: {arch_data.get('protocol', 'limine')}")
            config_lines.append("")  # Blank line for separation

            # Add modules with 'boot():' prefix and remove 'iso/' from paths
            for module in arch_data.get("files", []):
                module_path = module.get('iso.path', '').replace('iso/', '')
                config_lines.append(f"    module_path: boot():/{module_path}")
                config_lines.append(f"    module_cmdline: {module.get('command.line', '')}")
            config_lines.append("")  # Blank line for separation

            # Write the configuration to the output file
            with open(output_conf_file, 'w') as f:
                f.write("\n".join(config_lines))

            print(f"Configuration for '{target_arch}' written to {output_conf_file}")

            # Create the ISO after generating the configuration
            create_iso(arch_data, image_name, target_arch)

        else:
            print(f"Architecture '{target_arch}' not found or is not a valid dictionary in the configuration.")

    except KeyError as e:
        print(f"KeyError: {e}. Please check the JSON structure.")
    except Exception as e:
        print(f"An unexpected error occurred: {e}")




def create_iso(arch_data, image_name, target_arch):
    # Remove and recreate iso_root directory
    iso_root = 'iso_root'
    os.system(f"rm -rf {iso_root}")
    os.makedirs(os.path.join(iso_root, 'boot', 'limine'), exist_ok=True)
    os.makedirs(os.path.join(iso_root, 'EFI', 'BOOT'), exist_ok=True)

    # Define kernel source and target directories
    kernel_src_dir = '../kernel'
    
    # Define the kernel file to copy based on architecture
    kernel_files_map = {
        'x86_64': 'x86-64-kernel.elf',
        'aarch64': 'aarch64-kernel.elf',
        'riscv64': 'riscv64-kernel.elf',
        'loongarch64': 'loongarch64-kernel.elf',
    }

    # Check if the target architecture has a defined kernel
    if target_arch in kernel_files_map:
        kernel_file = kernel_files_map[target_arch]
        kernel_source_path = os.path.join(kernel_src_dir, kernel_file)

        # Copy the appropriate kernel file to the iso_root
        os.system(f"cp -v {kernel_source_path} {os.path.join(iso_root, kernel_file)}")
    else:
        print(f"No kernel file defined for architecture '{target_arch}'")

    # Copy the configuration file
    os.system(f"cp -v limine.conf {os.path.join(iso_root, 'boot', 'limine', 'limine.conf')}")

    # Copy architecture-specific EFI files to the correct path
    for efi_file in arch_data.get("efi.files", []):
        destination_path = os.path.join(iso_root, 'EFI', 'BOOT', os.path.basename(efi_file['iso.destination']))
        os.makedirs(os.path.dirname(destination_path), exist_ok=True)  # Ensure the directory exists
        os.system(f"cp -v {efi_file['path']} {destination_path}")

    # Handle specific architectures
    if target_arch == 'x86_64':
        os.system(f"cp -v limine/limine-bios.sys {os.path.join(iso_root, 'boot', 'limine')}")
        os.system(f"cp -v limine/limine-bios-cd.bin {os.path.join(iso_root, 'boot', 'limine')}")
        os.system(f"cp -v limine/limine-uefi-cd.bin {os.path.join(iso_root, 'boot', 'limine')}")
        
        # Create ISO with the correct extensions
        subprocess.run(['xorriso', '-as', 'mkisofs', '-b', 'boot/limine/limine-bios-cd.bin',
                        '-no-emul-boot', '-boot-load-size', '4', '-boot-info-table',
                        '--efi-boot', 'boot/limine/limine-uefi-cd.bin',
                        '-efi-boot-part', '--efi-boot-image', '--protective-msdos-label',
                        '-r', '-J', '-joliet-long', '-V', 'Kitty OS', 
                        iso_root, '-o', f'{image_name}.iso'])
        os.system(f"./limine/limine bios-install {image_name}.iso")

    elif target_arch in ['aarch64', 'riscv64', 'loongarch64']:
        os.system(f"cp -v limine/limine-uefi-cd.bin {os.path.join(iso_root, 'boot', 'limine')}")
        subprocess.run(['xorriso', '-as', 'mkisofs',
                        '--efi-boot', 'boot/limine/limine-uefi-cd.bin',
                        '-efi-boot-part', '--efi-boot-image', '--protective-msdos-label',
                        '-r', '-J', '-joliet-long', '-V', 'Kitty OS', 
                        iso_root, '-o', f'{image_name}.iso'])

    # Clean up the iso_root directory
    os.system(f"rm -rf {iso_root}")

generate_lim_config_and_iso('config.json', 'limine.conf', 'x86_64-kitty', 'x86_64')
generate_lim_config_and_iso('config.json', 'limine.conf', 'aarch64-kitty', 'aarch64')
generate_lim_config_and_iso('config.json', 'limine.conf', 'loongarch64-kitty', 'loongarch64')
generate_lim_config_and_iso('config.json', 'limine.conf', 'riscv64-kitty', 'riscv64')
