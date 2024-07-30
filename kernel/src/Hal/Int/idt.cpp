//
// Created by Piotr on 27.07.2024.
//

#include <Io/cpu.hpp>
#include "idt.hpp"

extern IDTGateDescriptor idt_table[256];

extern "C" void HalInterruptHandler(InterruptFrame* frame)
{
    IoHaltProcessor();
}

extern "C" void HalFlushInterruptDescriptorTableImpl(IDTRegister* idt_register);

void HalLoadInterruptDescriptorTable()
{
    IDTRegister idt_register = {
            .limit = sizeof(idt_table) - 1,
            .address = (uint64_t)&idt_table
    };

    HalFlushInterruptDescriptorTableImpl(&idt_register);
}