/**
 * @file        loggable.hpp
 * @brief       Defines Logger concept - compile time interface for a Logger.
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#ifndef LOGGABLE_HPP
#define LOGGABLE_HPP

#include <concepts>
#include <string>

enum class LogLevel
{
    info,
    warning,
    error
};

template<typename T>
concept Loggable = requires(T v, unsigned unsigned_, int int_, float float_, unsigned char unsigned_char_, char char_)
{
    v.log(LogLevel{}) << "string literal" //
                      << unsigned_        //
                      << int_             //
                      << float_           //
                      << unsigned_char_   //
                      << char_;           //
};

#endif /* LOGGABLE_HPP */