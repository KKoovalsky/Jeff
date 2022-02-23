/**
 * @file    multiple_instances_sequentially.cpp
 * @brief   Uses multiple instances of SerialLogger sequentially.
 * @author  Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#include "serial_logger.hpp"

void test_multiple_instances_working_sequentially()
{
    {
        SerialLogger logger1{};
        logger1.log(LogLevel::info) << "log from logger1";
    }

    {
        SerialLogger logger2{};
        logger2.log(LogLevel::info) << "log from logger2";
    }
}
