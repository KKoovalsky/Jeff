
#include "sampling_trigger_timer_impl.hpp"

#include "tim.h"

SamplingTriggerTimerImpl::SamplingTriggerTimerImpl()
{
    MX_TIM6_Init();
}

SamplingTriggerTimerImpl::~SamplingTriggerTimerImpl()
{
    LL_TIM_DeInit(TIM6);
}

void SamplingTriggerTimerImpl::start()
{
    LL_TIM_EnableCounter(TIM6);
}

void SamplingTriggerTimerImpl::stop()
{
    LL_TIM_DisableCounter(TIM6);
}
