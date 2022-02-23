/**
 * @file    chained_operations_printed.cpp
 * @brief   Chains multiple various operations to check whether they are properly printed.
 * @author  Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#include "serial_logger.hpp"

void test_chained_operations()
{
    SerialLogger serial_logger{};
    float f{3222.22};
    unsigned char hex_char{0x22};
    serial_logger.log(LogLevel::info) << "Some number: " << 190222 << ", some float: " << f
                                      << ", some other stuff: " << hex_char << ' ' << 'c' << ' ' << -1111;
}
