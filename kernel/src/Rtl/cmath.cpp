//
// Created by Piotr on 07.08.2024.
//

#include <ccmath/ccmath.hpp>
#include <Ke/debug.hpp>
#include <math.h>

constexpr double PI = 3.14159265358979323846;

extern "C"
{

float floorf(float _X)
{
    return ccm::floorf(_X);
}

float fmodf(float _X, float _Y)
{
    return ccm::fmodf(_X, _Y);
}

// Fishy! Check this
float sqrtf(float _X)
{
    return ccm::sqrtf(_X);
}

float powf(float _Base, float _Exp)
{
    return ccm::pow(_Base, _Exp);
}


float acosf(float x) {
    if (x < -1.0f || x > 1.0f)
    {
        return 0.0f;
    }

    float negate = x < 0.0f ? 1.0f : 0.0f;
    x = x < 0.0f ? -x : x;
    float ret = -0.0187293f;
    ret = ret * x;
    ret = ret + 0.0742610f;
    ret = ret * x;
    ret = ret - 0.2121144f;
    ret = ret * x;
    ret = ret + 1.5707288f;
    ret = ret * sqrtf(1.0f - x);
    ret = ret - 2.0f * negate * ret;
    return negate * PI + ret;
}

float cosf(float x) {
    float x2 = x * x;
    float result = 1.0f;
    float term = 1.0f;
    int i;

    for (i = 1; i < 10; i++)
    {
        term *= -x2 / (2 * i * (2 * i - 1));
        result += term;
    }

    return result;
}

double cos(double x) {
    double x2 = x * x;
    double result = 1.0;
    double term = 1.0;
    int i;

    for (i = 1; i < 10; i++)
    {
        term *= -x2 / (2 * i * (2 * i - 1));
        result += term;
    }

    return result;
}

float ceilf(float x)
{
    int integer_part = (int) x;
    if (x > (float) integer_part)
    {
        return (float) (integer_part + 1);
    } else
    {
        return (float) integer_part;
    }
}

}