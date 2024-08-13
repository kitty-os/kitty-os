#pragma once

#include <type_traits>

namespace Math
{
    template<typename T>
    concept Arithmetic = std::is_arithmetic_v<T>;

    template<Arithmetic T>
    constexpr T abs(T value)
    {
        return value<T(0) ? -value : value;
    }

    template<Arithmetic T>
    constexpr T min(T a, T b)
    {
        return (a<b) ? a : b;
    }

    template<Arithmetic T>
    constexpr T max(T a, T b)
    {
        return (a > b) ? a : b;
    }
}