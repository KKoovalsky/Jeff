/**
 * @file    benchmark_timer.hpp
 * @brief   Defines timer for benchmarking.
 * @author  Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#ifndef BENCHMARK_TIMER_HPP
#define BENCHMARK_TIMER_HPP

#include <atomic>

#include "jungles/singleton.hpp"

extern "C" void TIM7_IRQHandler();

class BenchmarkTimer : public jungles::Singleton<BenchmarkTimer>
{
  public:
    explicit BenchmarkTimer();
    ~BenchmarkTimer();

    unsigned time_microseconds() const;

  private:
    static void start();
    static void stop();

    const unsigned max_hardware_timer_counter_value;

    static inline std::atomic<unsigned> timer_counter_overflow_counter{0};

    friend void TIM7_IRQHandler();
};

#endif /* BENCHMARK_TIMER_HPP */
