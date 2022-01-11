/**
 * @file    test_audio_dac_basic.cpp
 * @brief   Tests Audio DAC implementation.
 * @author  Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#include "unity.h"

#include "audio_dac_impl.hpp"
#include "jungles_os_helpers/freertos/flag.hpp"
#include "os_waiters.hpp"

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
    {
        AudioDacImpl dac;
        dac.start();
        os::wait(100ms);
        dac.stop();
    }

    {
        AudioDacImpl dac;
        dac.start();
        os::wait(100ms);
        dac.stop();
    }
}

void test_single_batch_is_streamed()
{
    jungles::freertos::flag stream_updated_flag;
    AudioDacImpl dac;
    dac.set_on_stream_update_handler([&]() {
        stream_updated_flag.set();
        return std::make_unique<AudioDacBuffer>();
    });
    dac.start();
    os::wait(100ms);
    dac.stop();
}

void test_multiple_batches_are_streamed()
{
    TEST_IGNORE();
}
