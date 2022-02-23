/**
 * @file        filter_cutoff_setter_clock.hpp
 * @brief       Implements the FilterCutoffSetterClock.
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#include "filter_cutoff_setter_clock.hpp"

#include "tim.h"

FilterCutoffSetterClock::FilterCutoffSetterClock()
{
    MX_TIM16_Init();
    start();
}

FilterCutoffSetterClock::~FilterCutoffSetterClock()
{
    stop();
    LL_TIM_DeInit(TIM16);
}

void FilterCutoffSetterClock::start()
{
    LL_TIM_CC_EnableChannel(TIM16, LL_TIM_CHANNEL_CH1);
    LL_TIM_EnableCounter(TIM16);
    LL_TIM_EnableAllOutputs(TIM16);
}

void FilterCutoffSetterClock::stop()
{
    LL_TIM_DisableAllOutputs(TIM16);
    LL_TIM_CC_DisableChannel(TIM16, LL_TIM_CHANNEL_CH1);
    LL_TIM_DisableCounter(TIM16);
}
