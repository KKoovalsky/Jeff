/**
 * @file        filter_cutoff_setter_clock.hpp
 * @brief       Defines a class generating clock to set cutoff frequency of post-DAC filter and anti-aliasing filter.
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#ifndef FILTER_CUTOFF_SETTER_CLOCK_HPP
#define FILTER_CUTOFF_SETTER_CLOCK_HPP

//! RAII-like module: constructor starts the clock and destructor tears it down. Clock frequency: 2 MHz.
class FilterCutoffSetterClock
{
  public:
    explicit FilterCutoffSetterClock();
    ~FilterCutoffSetterClock();

  private:
    static void start();
    static void stop();
};

#endif /* FILTER_CUTOFF_SETTER_CLOCK_HPP */
