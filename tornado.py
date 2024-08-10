import subprocess

# Constants
START_ADDRESS = 0xffffffff80000000
END_ADDRESS = START_ADDRESS + 0x100000  # 1MB = 0x100000 bytes

def addr2line_lookup(addr):
    """Run addr2line for a given address and return the output."""
    addr_str = f"{addr:#x}"
    # Run the addr2line command
    result = subprocess.run(
        ['addr2line', '-e', 'kernel/bin/kernel', '-f', '-C', addr_str],
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True
    )
    # Split the output by lines (function name and file:line)
    output_lines = result.stdout.splitlines()
    if len(output_lines) >= 2:
        function_name = output_lines[0]
        file_line = output_lines[1]
        return function_name, file_line
    return None, "??:0"

def main():
    valid_entries = []
    for addr in range(START_ADDRESS, END_ADDRESS):
        function_name, file_line = addr2line_lookup(addr)
        if file_line != "??:0":
            valid_entries.append((function_name, file_line))
            print(f"Address: {addr:#x}, Function: {function_name}, File: {file_line}")
    
    print("\nTotal valid entries found:", len(valid_entries))

if __name__ == "__main__":
    main()

