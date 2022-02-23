/**
 * @file        button.hpp
 * @brief       Defines Button class.
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <chrono>

class Button
{
  public:
    template<class Duration>
    bool wait_for_press(Duration timeout)
    {
        auto timeout_ms{std::chrono::duration_cast<std::chrono::milliseconds>(timeout)};
        return wait_milliseconds_for_press(timeout_ms.count());
    }

  private:
    static bool wait_milliseconds_for_press(unsigned timeout_milliseconds);
};

#endif /* BUTTON_HPP */
