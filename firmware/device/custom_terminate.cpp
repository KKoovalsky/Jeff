/**
 * @file    custom_terminate.cpp
 * @brief   Defines custom terminate function to spare FLASH size.
 * @author  Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

// Enable this only for GCC to avoid multiple definitions of symbol __cxxabiv1::__terminate_handler when using Clang.
#ifndef __clang__

#include <exception>

[[noreturn]] void terminate() noexcept
{
    while (true)
        ;
}

namespace __cxxabiv1
{
std::terminate_handler __terminate_handler = terminate;
}

#endif