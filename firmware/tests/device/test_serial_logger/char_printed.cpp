/**
 * @file    char_printed.cpp
 * @brief   Prints chars with SerialLogger.
 * @author  Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#include "serial_logger.hpp"

void test_single_char_is_printed()
{
    SerialLogger serial_logger{};
    serial_logger.log(LogLevel::info) << 'a' << ' ' << '*';
}
