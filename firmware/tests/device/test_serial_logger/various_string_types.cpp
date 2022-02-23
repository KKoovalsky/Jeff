/**
 * @file        test_various_string_types.cpp
 * @brief       Various string types are printed by serial logger.
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#include "serial_logger.hpp"

#include <string>
#include <string_view>

void test_various_string_types()
{
    SerialLogger{}.log(LogLevel::info) << std::string{"string"} << ' ' << std::string_view{"string_view"}
                                       << " string_literal";
}