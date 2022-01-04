/**
 * @file    test_sample_sine_10khz.cpp
 * @brief   Tests whether the input signal has 10 kHz
 * @author  Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#include "test_sample_sine.hpp"

void test_10kHz_sine_wave_sampling()
{
    assert_input_signal_matches_sine(10000);
}
