/**
 * @file        os_waiters.hpp
 * @brief       Defines wait function(s) for OS layer.
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#ifndef WAITERS_HPP
#define WAITERS_HPP

#include <chrono>

/**
 * TODO: Shall be part of jungles_os_helpers. The implementation shall be dependent on the OS chosen. There shall be
 * a static library per each OS. User can choose the implementation basing on the CMake target chosen. Ideally, for
 * each library the header shall be the same.
 */
namespace os
{

void wait_milliseconds(unsigned milliseconds_to_wait);

template<typename Duration>
void wait(Duration duration)
{
    auto milliseconds{std::chrono::duration_cast<std::chrono::milliseconds>(duration)};
    wait_milliseconds(milliseconds.count());
}

//! Blocks allowing to run other threads.
[[noreturn]] void loop();

} // namespace os

#endif /* WAITERS_HPP */
