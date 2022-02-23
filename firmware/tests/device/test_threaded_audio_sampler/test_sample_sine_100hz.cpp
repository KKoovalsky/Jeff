/**
 * @file    test_sample_sine_100hz.cpp
 * @brief   Tests whether the input signal has 100 Hz
 * @author  Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#include "test_sample_sine.hpp"

void test_100Hz_sine_wave_sampling()
{
    assert_input_signal_matches_sine(100);
}
