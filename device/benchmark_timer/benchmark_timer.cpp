/**
 * @file    benchmark_timer.cpp
 * @brief   Implements BenchmarkTimer.
 * @author  Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#include "benchmark_timer.hpp"

#include "tim.h"

#include "os_waiters.hpp"

#define TIMER_HW_INSTANCE TIM7

BenchmarkTimer::BenchmarkTimer()
{
    MX_TIM7_Init();
    max_hardware_timer_counter_value = LL_TIM_GetAutoReload(TIMER_HW_INSTANCE) + 1;
    start();
}

BenchmarkTimer::~BenchmarkTimer()
{
    stop();
    LL_TIM_DeInit(TIMER_HW_INSTANCE);
}

void BenchmarkTimer::start()
{
    LL_TIM_EnableIT_UPDATE(TIMER_HW_INSTANCE);
    LL_TIM_SetCounter(TIMER_HW_INSTANCE, 0);
    LL_TIM_EnableCounter(TIMER_HW_INSTANCE);

    os::wait_milliseconds(1);
}

void BenchmarkTimer::stop()
{
    LL_TIM_DisableIT_UPDATE(TIMER_HW_INSTANCE);
    LL_TIM_DisableCounter(TIMER_HW_INSTANCE);
}

unsigned BenchmarkTimer::time_microseconds() const
{
    // BEWARE: obtaining those two values is not atomic and interrupt may happen at (1) (in between the loads of the
    // values. This may lead to improper values.
    // TODO: We may have to figure out a workaround here, e.g. using FreeRTOS functions like, e.g. try to obtain the
    // mutex; if can't then do some post-alignment, etc.
    auto overflow_count{timer_counter_overflow_counter.load()};
    // (1)
    auto current_timer_value{LL_TIM_GetCounter(TIMER_HW_INSTANCE)};

    // The hardware counter is incremented each 1 us (configured via CubeMx).
    unsigned result{overflow_count * max_hardware_timer_counter_value + current_timer_value};
    return result;
}

extern "C" void TIM7_IRQHandler()
{
    if (LL_TIM_IsActiveFlag_UPDATE(TIMER_HW_INSTANCE))
    {
        LL_TIM_ClearFlag_UPDATE(TIMER_HW_INSTANCE);
        ++BenchmarkTimer::timer_counter_overflow_counter;
    }
}
