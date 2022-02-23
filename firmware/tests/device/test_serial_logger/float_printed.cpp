/**
 * @file    float_printed.cpp
 * @brief   Prints float number using serial_logger.
 * @author  Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#include "serial_logger.hpp"

void test_float_is_printed_with_precision_to_thousandths()
{
    SerialLogger serial_logger{};
    serial_logger.log(LogLevel::info) << static_cast<float>(3.44422);
    serial_logger.log(LogLevel::info) << static_cast<float>(312.444888);
}
