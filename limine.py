import os

def create_limine_config(compiled_drivers_path, output_path):
    timeout = 3
    entries = [
        {
            "name": "Limine Template",
            "protocol": "limine",
            "kaslr": "no",
            "kernel_path": "boot:///boot/kernel",
            "modules": []
        },
        {
            "name": "Limine Template (with KASLR)",
            "protocol": "limine",
            "kernel_path": "boot:///boot/kernel",
            "modules": []
        }
    ]

    # Get all driver files from the compiled_drivers directory
    driver_files = os.listdir(compiled_drivers_path)
    for driver in driver_files:
        if os.path.isfile(os.path.join(compiled_drivers_path, driver)):
            module_entry = {
                "module_path": f"boot:///{driver}",
                "module_cmdline": driver
            }
            entries[0]["modules"].append(module_entry)

    # Create the limine.cfg content
    config_lines = [f"# Timeout in seconds that Limine will use before automatically booting.",
                    f"TIMEOUT={timeout}",
                    ""]

    for entry in entries:
        config_lines.append(f":{entry['name']}")
        config_lines.append(f"    PROTOCOL={entry['protocol']}")
        if "kaslr" in entry:
            config_lines.append(f"    KASLR={entry['kaslr']}")
        for module in entry["modules"]:
            config_lines.append(f"    MODULE_PATH={module['module_path']}")
            config_lines.append(f"    MODULE_CMDLINE={module['module_cmdline']}")
        config_lines.append(f"    KERNEL_PATH={entry['kernel_path']}")
        config_lines.append("")

    # Write the config to the output path
    with open(output_path, 'w') as config_file:
        config_file.write('\n'.join(config_lines))

# Define the paths
compiled_drivers_path = 'compiled_drivers'
output_path = 'limine.cfg'

# Create the config
create_limine_config(compiled_drivers_path, output_path)
print(f"Limine config written to {output_path}")

