/**
 * @file    test_benchmark_timer.cpp
 * @brief   Tests the benchmark timer.
 * @author  Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#include "unity.h"

#include "benchmark_timer.hpp"
#include "os_waiters.hpp"

/**
 * @defgroup BenchmarkTimerTests     Verifiers of proper measurement times.
 * @{
 */
void test_benchmark_timer_measures_1ms();
void test_benchmark_timer_measures_few_microseconds();
void test_benchmark_timer_measures_multiple_milliseconds();

/** @} */ // End of group BenchmarkTimerTests

void test_benchmark_timer_measures_1ms()
{
    BenchmarkTimer timer;

    auto begin_us{timer.time_microseconds()};
    os::wait_milliseconds(1);
    auto end_us{timer.time_microseconds()};

    unsigned duration_us{end_us - begin_us};
    TEST_ASSERT_UINT_WITHIN(50, 1000, duration_us);
}

void test_benchmark_timer_measures_few_microseconds()
{
    auto wait_10_microseconds{[]() {
        // BEWARE: changes in this code may affect timings. This is calculated for the Release build.
        // To generate dissasembly:
        //    ./common_dependencies/armgnutoolchain-src/bin/arm-none-eabi-objdump -d
        //        tests/device/test_benchmark_timer/test_benchmark_timer > test_benchmark_timer.disass
        extern uint32_t SystemCoreClock;
        const auto clock_frequency{static_cast<float>(SystemCoreClock)};
        // ADDS: 1 cycle + CMP: 1 cycle + BNE (conditional branch): 2 cycles = 4 cycles
        constexpr float cycles_per_loop{4.};
        const float loop_duration_seconds{cycles_per_loop / clock_frequency};
        constexpr float preamble_cycles{70};
        const float preamble_duration_seconds{preamble_cycles / clock_frequency};
        const float total_duration_seconds{0.000010};
        const unsigned number_of_loops{static_cast<unsigned>(
            ((total_duration_seconds - preamble_duration_seconds) / loop_duration_seconds) + 0.5f)};
        for (unsigned i{0}; i < number_of_loops; ++i)
            // Prevent from optimizing out.
            __asm__ __volatile__("");
    }};

    BenchmarkTimer timer;

    auto begin_us{timer.time_microseconds()};
    wait_10_microseconds();
    auto end_us{timer.time_microseconds()};

    unsigned duration_us{end_us - begin_us};
    TEST_ASSERT_UINT_WITHIN(1, 10, duration_us);
}

void test_benchmark_timer_measures_multiple_milliseconds()
{
    BenchmarkTimer timer;

    auto begin_us{timer.time_microseconds()};
    os::wait_milliseconds(20);
    auto end_us{timer.time_microseconds()};

    unsigned duration_us{end_us - begin_us};
    TEST_ASSERT_UINT_WITHIN(400, 20000, duration_us);
}
