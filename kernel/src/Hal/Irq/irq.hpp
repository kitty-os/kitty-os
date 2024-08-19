#pragma once

#include <Hal/Int/idt.hpp>
#include <functional>

bool HalIrqRegisterISAIRQ(int irq, std::function<void(InterruptFrame* _If)>);
bool HalIrqRegisterGSIIRQ(int gsi, std::function<void(InterruptFrame* _If)>);
bool HalIrqRegisterISAIRQSpecificCPU(int irq, std::function<void(InterruptFrame* _If)>);
bool HalIrqRegisterGSIIRQSpecificCPU(int gsi, std::function<void(InterruptFrame* _If)>);
void HalIrqInitializeAdvancedIRQs();