/**
 * @file    hard_fault_handler.cpp
 * @brief   Defines the HardFault_Handler.
 * @author  Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#include <cstdint>

struct __attribute__((packed)) ContextStateFrame
{
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r12;
    uint32_t lr;
    uint32_t return_address;
    uint32_t xpsr;
};

extern "C" void HardFault_Handler()
{
    __asm volatile(
        "tst lr, #4 \n"
        "ite eq \n"
        "mrseq r0, msp \n"
        "mrsne r0, psp \n"
        "b hard_fault_handler \n");
}

extern "C" void hard_fault_handler([[maybe_unused]] ContextStateFrame* frame)
{
    do
    {
        __asm volatile("nop \n");
    } while (true);
}