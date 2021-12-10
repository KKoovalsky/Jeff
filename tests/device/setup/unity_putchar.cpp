/**
 * @file    unity_putchar.cpp
 * @brief   Defines outputting function used by Unity test library.
 * @author  Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#include "serial_logger.hpp"

extern "C" void unity_putchar(int c)
{
    static SerialLogger logger;
    logger.log_raw(static_cast<unsigned char>(c));
}
