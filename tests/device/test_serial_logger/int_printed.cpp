/**
 * @file    int_printed.cpp
 * @brief   Prints integers, positive and negative, to the serial output.
 * @author  Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#include "serial_logger.hpp"

void test_ints_are_printed()
{
    SerialLogger serial_logger{};
    serial_logger.log(LogLevel::info) << static_cast<int>(32000);
    serial_logger.log(LogLevel::info) << static_cast<int>(-32311);
}