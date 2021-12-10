/**
 * @file    test_threaded_audio_sampler.cpp
 * @brief   Tests the ThreadedAudioSampler.
 * @author  Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#include "threaded_audio_sampler.hpp"

#include "jungles_os_helpers/freertos/flag.hpp"

#include "unity.h"

/**
 * @defgroup ThreadedAudioSamplerTests     Tests of ThreadedAudioSampler
 * @{
 */
void test_single_sample_is_obtained();
void test_proper_number_of_samples_is_collected_within_specific_period();
void test_sampling_frequency_is_stable();
void test_cant_create_two_instances();
/** @} */ // End of group ThreadedAudioSamplerTests

// =====================================================================================================================
// Tests' definition
// =====================================================================================================================

void test_single_sample_is_obtained()
{
    jungles::freertos::flag sample_received_flag;

    ThreadedAudioSampler sampler;
    sampler.set_on_sample_received_handler([&](int) { sample_received_flag.set(); });
    sampler.start();

    sample_received_flag.wait();
    sampler.stop();

    TEST_ASSERT_TRUE(true);
}

void test_proper_number_of_samples_is_collected_within_specific_period()
{
}

void test_sampling_frequency_is_stable()
{
}

void test_cant_create_two_instances()
{
}
