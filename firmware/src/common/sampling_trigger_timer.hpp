/**
 * @file        sampling_trigger_timer.hpp
 * @brief       Interface for a timer that triggers sampling.
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#ifndef SAMPLING_TRIGGER_TIMER_HPP
#define SAMPLING_TRIGGER_TIMER_HPP

struct SamplingTriggerTimer
{
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual ~SamplingTriggerTimer() = default;
};

#endif /* SAMPLING_TRIGGER_TIMER_HPP */
