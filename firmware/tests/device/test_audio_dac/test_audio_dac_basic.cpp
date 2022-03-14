/**
 * @file    test_audio_dac_basic.cpp
 * @brief   Tests Audio DAC implementation.
 * @author  Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#include "unity.h"

#include "benchmark_timer.hpp"
#include "os_waiters.hpp"
#include "sampling_trigger_timer_impl.hpp"
#include "audio_dac_with_dma.hpp"

#include "dummy_event_tracer.hpp"

#include <chrono>
#include <memory>

/**
 * @defgroup AudioDacTests     Declaration of the tests of DAC.
 * @{
 */
void test_two_instances_work_one_after_another();
void test_single_batch_is_streamed();
void test_multiple_batches_are_streamed();
/** @} */ // End of group AudioDacTests

using namespace std::chrono_literals;

// =====================================================================================================================
// Tests' definition
// =====================================================================================================================
void test_two_instances_work_one_after_another()
{
    SamplingTriggerTimerImpl sampling_trigger_timer;
    DummyEventTracer event_tracer;
    {
        AudioDacWithDma dac{sampling_trigger_timer, event_tracer};
        os::wait(100ms);
    }

    {
        AudioDacWithDma dac{sampling_trigger_timer, event_tracer};
        os::wait(100ms);
    }

    TEST_ASSERT(true);
}

void test_single_batch_is_streamed()
{
    SamplingTriggerTimerImpl sampling_trigger_timer;
    DummyEventTracer event_tracer;

    AudioDacWithDma dac{sampling_trigger_timer, event_tracer};
    dac.await_stream_update(AudioChainConfig::BatchOfSamples{});

    TEST_ASSERT(true);
}

void test_multiple_batches_are_streamed()
{
    SamplingTriggerTimerImpl sampling_trigger_timer;
    DummyEventTracer event_tracer;

    constexpr unsigned sampling_frequency_hz{44100};
    unsigned number_of_batches_within_one_second{sampling_frequency_hz / AudioChainConfig::WindowSize};

    AudioDacWithDma dac{sampling_trigger_timer, event_tracer};

    BenchmarkTimer timer;
    auto start_us{timer.time_microseconds()};
    for (unsigned i{0}; i < number_of_batches_within_one_second; ++i)
        dac.await_stream_update(AudioChainConfig::BatchOfSamples{});
    auto end_us{timer.time_microseconds()};

    auto duration_us{end_us - start_us};
    TEST_ASSERT_UINT_WITHIN(20000, 1000000, duration_us);
}
