#pragma once

#include <limine/limine.h>
#include <functional>
#include <cstddef>

template <typename T>
struct Vector2
{
    T x, y;

    Vector2() : x(T()), y(T()) {}
    Vector2(T x, T y) : x(x), y(y) {}

    Vector2<T> operator+(const Vector2<T>& other) const {
        return Vector2<T>(x + other.x, y + other.y);
    }

    Vector2<T> operator-(const Vector2<T>& other) const {
        return Vector2<T>(x - other.x, y - other.y);
    }

    Vector2<T> operator*(T scalar) const {
        return Vector2<T>(x * scalar, y * scalar);
    }

    Vector2<T> operator/(T scalar) const {
        return Vector2<T>(x / scalar, y / scalar);
    }

    T operator*(const Vector2<T>& other) const {
        return x * other.x + y * other.y;
    }

    bool operator==(const Vector2<T>& other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Vector2<T>& other) const {
        return (*this != other);
    }

    Vector2<T> operator-() const {
        return Vector2<T>(-x, -y);
    }
};

template <typename T>
struct Vector3
{
    T x, y, z;

    Vector3() : x(T()), y(T()), z(T()) {}
    Vector3(T x, T y, T z) : x(x), y(y), z(z) {}

    Vector3<T> operator+(const Vector3<T>& other) const {
        return Vector3<T>(x + other.x, y + other.y, z + other.z);
    }

    Vector3<T> operator-(const Vector3<T>& other) const {
        return Vector3<T>(x - other.x, y - other.y, z - other.z);
    }

    Vector3<T> operator*(T scalar) const {
        return Vector3<T>(x * scalar, y * scalar, z * scalar);
    }

    Vector3<T> operator/(T scalar) const {
        return Vector3<T>(x / scalar, y / scalar, z / scalar);
    }

    T operator*(const Vector3<T>& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    Vector3<T> cross(const Vector3<T>& other) const {
        return Vector3<T>(
                y * other.z - z * other.y,
                z * other.x - x * other.z,
                x * other.y - y * other.x
        );
    }

    bool operator==(const Vector3<T>& other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    bool operator!=(const Vector3<T>& other) const {
        return (*this != other);
    }

    Vector3<T> operator-() const {
        return Vector3<T>(-x, -y, -z);
    }
};

void GfxDrawPixel(
        limine_framebuffer* framebuffer,
        Vector2<size_t> position,
        Vector3<uint8_t> color
        );

void GfxDrawPixelNoOutOfBounds(
    limine_framebuffer* framebuffer,
    Vector2<size_t> position,
    Vector3<uint8_t> color
);

void GfxDrawPixelFragShadered(
        limine_framebuffer* framebuffer,
        Vector2<size_t> position,
        Vector3<uint8_t> color,
        std::function<void(const Vector2<size_t>& position, Vector3<uint8_t>&)> basic_shader
        );

void GfxDrawRectangle(
        limine_framebuffer* framebuffer,
        Vector2<size_t> position,
        Vector3<uint8_t> color,
        Vector2<size_t> dimensions
        );

void GfxDrawRectangleFragShadered(
        limine_framebuffer* framebuffer,
        Vector2<size_t> position,
        Vector3<uint8_t> color,
        Vector2<size_t> dimensions,
        std::function<void(const Vector2<size_t>& position, Vector3<uint8_t>& pixel)> basic_shader
        );

void GfxDrawLine(
        limine_framebuffer* framebuffer,
        Vector2<size_t> start,
        Vector2<size_t> end,
        Vector3<uint8_t> color
);