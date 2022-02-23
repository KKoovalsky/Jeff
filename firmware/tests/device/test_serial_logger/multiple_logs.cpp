/**
 * @file    multiple_logs.cpp
 * @brief   Prints multiple lines over serial.
 * @author  Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#include "serial_logger.hpp"

void test_multiple_logs_are_printed()
{
    SerialLogger serial_logger{};
    serial_logger.log(LogLevel::info) << "some log 1";
    serial_logger.log(LogLevel::info) << "some log 2";
    serial_logger.log(LogLevel::info) << "some log 3";
    serial_logger.log(LogLevel::info) << "some log 4";
    serial_logger.log(LogLevel::info) << "some log 5";
    serial_logger.log(LogLevel::info) << "some log 6";
}
