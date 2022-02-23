/**
 * @file        sampling_trigger_timer_impl.hpp
 * @brief       Implements SamplingTriggerTimer.
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#ifndef SAMPLING_TRIGGER_TIMER_IMPL_HPP
#define SAMPLING_TRIGGER_TIMER_IMPL_HPP

#include "sampling_trigger_timer.hpp"

class SamplingTriggerTimerImpl : public SamplingTriggerTimer
{
  public:
    explicit SamplingTriggerTimerImpl();
    ~SamplingTriggerTimerImpl() override;
    void start() override;
    void stop() override;
};

#endif /* SAMPLING_TRIGGER_TIMER_IMPL_HPP */
