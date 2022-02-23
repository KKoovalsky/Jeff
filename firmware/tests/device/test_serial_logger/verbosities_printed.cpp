/**
 * @file    verbosities_printed.cpp
 * @brief   Check various verbosities are printed with SerialLogger.
 * @author  Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#include "serial_logger.hpp"

void test_verbosities_are_printed()
{
    SerialLogger serial_logger{};
    serial_logger.log(LogLevel::info) << "info";
    serial_logger.log(LogLevel::warning) << "warning";
    serial_logger.log(LogLevel::error) << "error";
}
