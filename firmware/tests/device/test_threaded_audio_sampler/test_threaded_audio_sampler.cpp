/**
 * @file    test_threaded_audio_sampler.cpp
 * @brief   Tests the ThreadedAudioSampler.
 * @author  Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#include <chrono>

#include "benchmark_timer.hpp"
#include "dummy_event_tracer.hpp"
#include "os_waiters.hpp"
#include "sampling_trigger_timer_impl.hpp"
#include "serial_logger.hpp"
#include "threaded_audio_sampler.hpp"

#include "unity.h"

using namespace std::chrono_literals;

/**
 * @defgroup ThreadedAudioSamplerTests     Tests of ThreadedAudioSampler
 * @{
 */
void test_single_batch_of_samples_is_obtained();
void test_multiple_instances_can_be_run_one_after_another();
void test_proper_number_of_samples_is_collected_within_specific_period();
void test_not_collecting_samples_doesnt_crash_the_app();
void test_large_time_gap_in_collecting_doesnt_crash_the_app();
/** @} */ // End of group ThreadedAudioSamplerTests

// =====================================================================================================================
// Tests' definition
// =====================================================================================================================

void test_single_batch_of_samples_is_obtained()
{
    SamplingTriggerTimerImpl sampling_trigger_timer;
    DummyEventTracer event_tracer;
    ThreadedAudioSampler sampler{sampling_trigger_timer, event_tracer};
    auto samples{sampler.await_samples()};
    TEST_ASSERT(samples[0] != 0.0 or samples[2] != 0.032);
}

void test_multiple_instances_can_be_run_one_after_another()
{
    SamplingTriggerTimerImpl sampling_trigger_timer;
    DummyEventTracer event_tracer;

    {
        ThreadedAudioSampler sampler{sampling_trigger_timer, event_tracer};
        auto samples{sampler.await_samples()};
        TEST_ASSERT(samples[0] != 0.0 or samples[2] != 0.032);
    }

    {
        ThreadedAudioSampler sampler{sampling_trigger_timer, event_tracer};
        auto samples{sampler.await_samples()};
        TEST_ASSERT(samples[0] != 0.0 or samples[2] != 0.032);
    }
}

void test_proper_number_of_samples_is_collected_within_specific_period()
{
    SamplingTriggerTimerImpl sampling_trigger_timer;
    DummyEventTracer event_tracer;
    ThreadedAudioSampler sampler{sampling_trigger_timer, event_tracer};

    constexpr unsigned sampling_frequency_hz{44100};
    unsigned number_of_batches_within_one_second{sampling_frequency_hz / AudioChainConfig::WindowSize};

    BenchmarkTimer timer;
    auto start_us{timer.time_microseconds()};
    for (unsigned i{0}; i < number_of_batches_within_one_second; ++i)
        sampler.await_samples();
    auto end_us{timer.time_microseconds()};

    auto duration_us{end_us - start_us};
    TEST_ASSERT_UINT_WITHIN(20000, 1000000, duration_us);
}

void test_not_collecting_samples_doesnt_crash_the_app()
{
    SamplingTriggerTimerImpl sampling_trigger_timer;
    DummyEventTracer event_tracer;
    ThreadedAudioSampler sampler{sampling_trigger_timer, event_tracer};
    os::wait_milliseconds(200);
    TEST_ASSERT(true);
}

void test_large_time_gap_in_collecting_doesnt_crash_the_app()
{
    SamplingTriggerTimerImpl sampling_trigger_timer;
    DummyEventTracer event_tracer;
    ThreadedAudioSampler sampler{sampling_trigger_timer, event_tracer};

    sampler.await_samples();
    sampler.await_samples();
    os::wait_milliseconds(200);
    sampler.await_samples();
    sampler.await_samples();
    sampler.await_samples();
    sampler.await_samples();

    TEST_ASSERT(true);
}
