/**
 * @file    unsigned_int_printed.cpp
 * @brief   Prints unsigned int with SerialLogger.
 * @author  Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#include "serial_logger.hpp"

void test_unsigned_int_is_printed()
{
    SerialLogger serial_logger{};
    serial_logger.log(LogLevel::info) << static_cast<unsigned>(10000);
}
