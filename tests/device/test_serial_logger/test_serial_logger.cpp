/**
 * @file    test_serial_logger.cpp
 * @brief   Tests the SerialLogger
 * @author  Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#include "unity.h"

#include "serial_logger.hpp"

// =====================================================================================================================
// Test cases declaration
// =====================================================================================================================
void test_implements_loggable();

// =====================================================================================================================
// Test cases definition
// =====================================================================================================================
void test_implements_loggable()
{
    static_assert(Loggable<SerialLogger>);
}