/**
 * @file    benchmark_timer.hpp
 * @brief   Defines timer for benchmarking.
 * @author  Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#ifndef BENCHMARK_TIMER_HPP
#define BENCHMARK_TIMER_HPP

class BenchmarkTimer
{
  public:
    explicit BenchmarkTimer();
    ~BenchmarkTimer();

    void start();
    void stop();
    unsigned time_from_start_in_microseconds();
};

#endif /* BENCHMARK_TIMER_HPP */
