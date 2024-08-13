//
// Created by Piotr on 07.08.2024.
//


extern "C"
{

static float factorial(int n)
{
    float result = 1.0;
    for (int i = 1; i <= n; ++i)
    {
        result *= i;
    }
    return result;
}

static double pow(double x, int y)
{
    double result = 1.0;
    for (int i = 0; i < y; i++)
    {
        result *= x;
    }
    return result;
}

float cosf(float x)
{
    float sum = 1.0; // Start with the first term
    float term = 1.0;
    int i = 0;

    while (1)
    {
        i += 2;
        term *= -x * x / ((i - 1) * i); // Compute next term
        if (term < 1e-6 && term > -1e-6)
        { // Small enough term
            break;
        }
        sum += term;
    }

    return sum;
}

float sinf(float x)
{
    float sum = x; // Start with the first term
    float term = x;
    int i = 1;

    while (1)
    {
        i += 2;
        term *= -x * x / (i * (i - 1)); // Compute next term
        if (term < 1e-6 && term > -1e-6)
        { // Small enough term
            break;
        }
        sum += term;
    }

    return sum;
}

int abs(int x)
{
    return (x < 0) ? -x : x;
}

float sqrtf(float x)
{
    if (x < 0) {
        return -1;
    }

    if (x == 0 || x == 1) {
        return x;
    }

    float guess = x / 2.0f;
    float epsilon = 0.00001f;

    while ((guess * guess - x) > epsilon || (x - guess * guess) > epsilon)
    {
        guess = (guess + x / guess) / 2.0f;
    }

    return guess;
}

float floorf(float x)
{
    int i = (int)x;

    if (x < 0 && x != i)
    {
        return i - 1;
    }

    return i;
}

float ceilf(float x)
{
    int i = (int)x;

    if (x > 0 && x != i)
    {
        return i + 1;
    }

    return i;
}


float fmodf(float x, float y)
{
    if (y == 0.0f)
    {
        return 0.0f;
    }

    int quotient = (int)(x / y);
    float remainder = x - (quotient * y);

    return remainder;
}

float acosf(float x)
{
    if (x < -1.0f || x > 1.0f)
    {
        return -1;
    }

    float negate = (x < 0) ? 1.0f : 0.0f;
    x = (x < 0) ? -x : x;

    float ret = -0.0187293f;
    ret = ret * x + 0.0742610f;
    ret = ret * x - 0.2121144f;
    ret = ret * x + 1.5707288f;
    ret = ret * sqrtf(1.0f - x);
    ret = ret - 2 * negate * ret;
    return negate * 3.14159265358979f + ret;
}

float power_int(float x, int n)
{
    float result = 1.0f;
    for (int i = 0; i < n; ++i)
    {
        result *= x;
    }
    return result;
}

float powf(float base, float exponent)
{
    if (base < 0)
    {
        if (exponent != (int)exponent)
        {
            return -1; // Error code
        }
    }

    if (exponent == (int)exponent)
    {
        return power_int(base, (int)exponent);
    }

    if (base == 0.0f)
    {
        return (exponent > 0) ? 0.0f : (exponent < 0) ? 1.0f / 0.0f : 1.0f; // Handle 0^positive, 0^negative, 0^0
    }

    if (base == 1.0f || exponent == 0)
    {
        return 1.0f;
    }

    return -1; // Basic implementation does not handle non-integer exponents or negative bases with non-integer exponents
}


double cos(double x)
{
    const int TERMS = 10;
    double sum = 0.0;

    for (int n = 0; n < TERMS; n++)
    {
        // Calculate (2n)!
        double fact = factorial(2 * n);

        // Calculate term (-1)^n * x^(2n) / (2n)!
        double term = (n % 2 == 0 ? 1 : -1) * pow(x, 2 * n) / fact;

        // Add term to sum
        sum += term;
    }

    return sum;
}

}