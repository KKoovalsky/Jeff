/**
 * @file    simple_log.cpp
 * @brief   Tests simple log is printed to serial.
 * @author  Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#include "serial_logger.hpp"

void test_simple_log_is_printed()
{
    SerialLogger{}.log(LogLevel::info) << "some simple log";
}
