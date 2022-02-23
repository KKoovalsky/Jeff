/**
 * @file    unsigned_char_printed_as_hex_char.cpp
 * @brief   Prints unsigned char with serial logger.
 * @author  Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#include "serial_logger.hpp"

void test_unsigned_char_is_printed_as_hex_char()
{
    SerialLogger serial_logger{};
    serial_logger.log(LogLevel::info) << static_cast<unsigned char>(0x02);
    serial_logger.log(LogLevel::info) << static_cast<unsigned char>(0xFA);
}
