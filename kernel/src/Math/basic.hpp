#pragma once

#include <type_traits>
#include <cmath>

// Helper to enable functions only for floating-point types
template <typename T, typename Enable = void>
struct IsFloatingPoint : std::false_type {};

template <typename T>
struct IsFloatingPoint<T, typename std::enable_if<std::is_floating_point<T>::value>::type> : std::true_type {};

// constexpr min function for arithmetic types
template <typename T>
constexpr typename std::enable_if<std::is_arithmetic<T>::value, T>::type
min(T a, T b)
{
    return (a < b) ? a : b;
}

// constexpr max function for arithmetic types
template <typename T>
constexpr typename std::enable_if<std::is_arithmetic<T>::value, T>::type
max(T a, T b)
{
    return (a < b) ? b : a;
}

// constexpr square root function using Newton-Raphson method
template <typename T>
constexpr typename std::enable_if<IsFloatingPoint<T>::value, T>::type
sqrt(T value, T guess = 1.0, T epsilon = 1e-6) {
T next_guess = (guess + value / guess) / 2.0;
    return (std::abs(next_guess - guess) < epsilon) ? next_guess : sqrt(value, next_guess, epsilon);
}

// constexpr power function for unsigned integer exponents
template <typename T>
constexpr typename std::enable_if<IsFloatingPoint<T>::value, T>::type
pow(T base, unsigned int exponent) {
    return (exponent == 0) ? 1 : base * pow(base, exponent - 1);
}

// Specialization for negative exponents
template <typename T>
constexpr typename std::enable_if<IsFloatingPoint<T>::value, T>::type
pow(T base, int exponent) {
    return (exponent < 0) ? 1 / pow(base, -exponent) : pow(base, static_cast<unsigned int>(exponent));
}

// constexpr factorial function
template <typename T>
constexpr typename std::enable_if<IsFloatingPoint<T>::value, T>::type
factorial(unsigned int n) {
    return (n == 0) ? 1 : n * factorial<T>(n - 1);
}

// constexpr sine function using Taylor series expansion
template <typename T>
constexpr typename std::enable_if<IsFloatingPoint<T>::value, T>::type
sin(T angle, unsigned int n = 10) {
    if (n == 0) return angle;

    T term = (n % 2 == 0 ? angle : -angle) / factorial<T>(2 * n + 1);
    return term + sin(angle, n - 1);
}

// constexpr cosine function using Taylor series expansion
template <typename T>
constexpr typename std::enable_if<IsFloatingPoint<T>::value, T>::type
cos(T angle, unsigned int n = 10) {
    if (n == 0) return 1;

    T term = (n % 2 == 0 ? pow(angle, 2 * n) : -pow(angle, 2 * n)) / factorial<T>(2 * n);
    return term + cos(angle, n - 1);
}