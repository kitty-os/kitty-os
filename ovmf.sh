#!/bin/bash

# Array of architectures
ARCHS=("x86_64" "aarch64" "loongarch64" "riscv64")

# Create the ovmf directory if it doesn't exist
mkdir -p ovmf

# Loop through each architecture
for KARCH in "${ARCHS[@]}"; do
    # Define the paths for code and vars
    CODE_PATH="ovmf/ovmf-code-${KARCH}.fd"
    VARS_PATH="ovmf/ovmf-vars-${KARCH}.fd"

    # Download ovmf-code binary
    echo "Downloading ovmf-code for ${KARCH}..."
    curl -Lo "${CODE_PATH}" "https://github.com/osdev0/edk2-ovmf-nightly/releases/latest/download/ovmf-code-${KARCH}.fd"

    # Adjust file size based on architecture
    case "${KARCH}" in
        aarch64)
            dd if=/dev/zero of="${CODE_PATH}" bs=1 count=0 seek=67108864 2>/dev/null
            ;;
        riscv64)
            dd if=/dev/zero of="${CODE_PATH}" bs=1 count=0 seek=33554432 2>/dev/null
            ;;
        # Add additional cases if needed for other architectures
    esac

    # Download ovmf-vars binary
    echo "Downloading ovmf-vars for ${KARCH}..."
    curl -Lo "${VARS_PATH}" "https://github.com/osdev0/edk2-ovmf-nightly/releases/latest/download/ovmf-vars-${KARCH}.fd"

    # Adjust file size based on architecture
    case "${KARCH}" in
        aarch64)
            dd if=/dev/zero of="${VARS_PATH}" bs=1 count=0 seek=67108864 2>/dev/null
            ;;
        riscv64)
            dd if=/dev/zero of="${VARS_PATH}" bs=1 count=0 seek=33554432 2>/dev/null
            ;;
        # Add additional cases if needed for other architectures
    esac
done

echo "All downloads completed."
