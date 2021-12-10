/**
 * @file    enormous_number_of_logs_doesnt_clog.cpp
 * @brief   Prints over 1000 lines of logs without a delay.
 * @author  Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#include "serial_logger.hpp"

void test_enormous_number_of_logs_doesnt_clog_the_logger()
{
    SerialLogger serial_logger{};
    for (unsigned i{0}; i < 1000; ++i)
    {
        serial_logger.log(LogLevel::info) << "some log, which is not so long";
    }
}
