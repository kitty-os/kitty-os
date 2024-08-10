//
// Created by Piotr on 08.08.2024.
//

#include <Ke/debug.hpp>
#include <cstddef>
#include "decompiler.hpp"

// ANSI escape codes for colors
#define RESET_COLOR "\033[0m"
#define RED_COLOR "\033[31m"
#define GREEN_COLOR "\033[32m"
#define YELLOW_COLOR "\033[33m"

void DecompilerDecompileAtRIPRange(xed_decoded_inst_t* significant_instruction, const uint8_t* rip, size_t range_before, size_t range_after)
{
    // Initialize XED tables
    xed_tables_init();

    // Set the machine state for 64-bit mode
    xed_state_t dstate;
    xed_state_zero(&dstate);
    xed_state_init(&dstate, XED_MACHINE_MODE_LONG_64, XED_ADDRESS_WIDTH_64b, XED_ADDRESS_WIDTH_64b);

    // Calculate the start and end of the decoding range
    const uint8_t* start = rip - range_before;
    const uint8_t* end = rip + range_after;

    // Start decoding at RIP - range_before
    uint64_t current_rip = reinterpret_cast<uint64_t>(start);

    // Decode instructions in the range RIP - range_before to RIP + range_after
    while (start < end)
    {
        // Set up the decoded instruction object
        xed_decoded_inst_t xedd;
        xed_decoded_inst_zero_set_mode(&xedd, &dstate);

        // Decode the instruction
        xed_error_enum_t xed_error = xed_decode(&xedd, start, end - start);

        if (xed_error == XED_ERROR_GENERAL_ERROR)
        {
            // Handle GENERAL_ERROR by incrementing RIP by one
            ++start;
            ++current_rip;
            DbgPrintf(YELLOW_COLOR "Skipped address 0x%llx due to GENERAL_ERROR\n" RESET_COLOR, current_rip - 1);
            continue;
        }
        else if (xed_error != XED_ERROR_NONE)
        {
            // For other errors, print the error message and stop
            DbgPrintf(RED_COLOR "Failed to decode instruction at 0x%llx: %s\n" RESET_COLOR, current_rip, xed_error_enum_t2str(xed_error));
            break;
        }

        // Get the instruction length
        unsigned int instr_length = xed_decoded_inst_get_length(&xedd);

        // Format the disassembled instruction
        char buffer[2048];
        xed_format_context(XED_SYNTAX_INTEL, &xedd, buffer, sizeof(buffer), current_rip, nullptr, nullptr);

        // Check if this instruction is at the RIP address (offset 0)
        if (start == rip)
        {
            *significant_instruction = xedd;
            // Print the instruction with '\t->' to indicate it's at the RIP
            DbgPrintf(GREEN_COLOR "  -> 0x%llx: %s\n" RESET_COLOR, current_rip, buffer);
        }
        else
        {
            // Print the instruction without special indication
            DbgPrintf(YELLOW_COLOR "0x%llx: %s\n" RESET_COLOR, current_rip, buffer);
        }

        // Move to the next instruction
        start += instr_length;
        current_rip += instr_length;
    }
}
