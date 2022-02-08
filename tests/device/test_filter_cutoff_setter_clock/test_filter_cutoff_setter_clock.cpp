/**
 * @file    test_filter_cutoff_setter_clock.cpp
 * @brief   Tests the Clock which sets the cutoff frequency for the post-DAC and anti-aliasing filter.
 * @author  Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#include "filter_cutoff_setter_clock.hpp"
#include "os_waiters.hpp"
#include "serial_logger.hpp"

void test_clock_signal_is_generated_to_set_the_filter_cutoff_frequency()
{
    SerialLogger logger;
    logger.log(LogLevel::info) << "Starting the clock generation. It shall be 2 Mhz.";

    FilterCutoffSetterClock clock;

    logger.log(LogLevel::info) << "Clock started ...";

    os::loop();
}
