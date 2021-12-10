/**
 * @file    multiple_instances_simultaneously.cpp
 * @brief   Writes logs to output with multiple instances.
 * @author  Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#include "serial_logger.hpp"

void test_multiple_instances_working_simultaneously()
{
    SerialLogger logger1{};
    SerialLogger logger2{};

    logger1.log(LogLevel::info) << "log from logger1";
    logger2.log(LogLevel::info) << "log from logger2";
}