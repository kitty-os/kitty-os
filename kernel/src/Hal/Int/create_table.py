with open("table.cpp", "w") as table:
    table.write("#include \"idt.hpp\"\n")

    # Create external definitions.
    for FunctionIndex in range(0, 256):
        table.write(f"extern \"C\" void InterruptHandler{FunctionIndex}(InterruptFrame* frame); \n")

    # Create the table for IDT
    table.write("IDTGateDescriptor idt_table[256] = {\n");
    for FunctionIndex in range(0, 256):
        table.write(f"IDT_ENTRY((uint64_t)&InterruptHandler{FunctionIndex},0x8,IDT_GATE_TYPE_INTERRUPT,0,0),")
    table.write("};\n");
