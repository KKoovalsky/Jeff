/**
 * @file    test_logs_uncaught_exception.cpp
 * @brief   Throws exception that shall be caught by the runner.
 * @author  Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#include <stdexcept>

void test_throws_uncaught_exception()
{
    throw std::runtime_error{"some exception message!"};
}