//
// Created by Piotr on 26.07.2024.
//

extern "C" void DriverEntry()
{
    asm volatile ("syscall");
    while (1);
}