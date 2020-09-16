#pragma once

#include "Hry/Math/Vec3.hpp"
#include "Hry/Namespace.hpp"

#include "fmt/core.h"

HRY_NS_BEGIN

template<typename T>
struct Vec3
{
    T x;
    T y;
    T z;

    Vec3() = default;
    Vec3(T x, T y, T z) : x(x), y(y), z(z) {}
    explicit Vec3(T value) : x(value), y(value), z(value) {}
};

template<typename T>
constexpr Vec3<T> operator-(const Vec3<T>& right)
{
    return { -right.x, -right.y, -right.z };
}

template<typename T>
constexpr Vec3<T>& operator+=(Vec3<T>& left, const Vec3<T>& right)
{
    left.x += right.x;
    left.y += right.y;
    left.z += right.z;

    return left;
}

template<typename T>
constexpr Vec3<T>& operator-=(Vec3<T>& left, const Vec3<T>& right)
{
    left.x -= right.x;
    left.y -= right.y;
    left.z -= right.z;

    return left;
}

template<typename T>
constexpr Vec3<T> operator+(const Vec3<T>& left, const Vec3<T>& right)
{
    return { left.x + right.x, left.y + right.y, left.z + right.z };
}

template<typename T>
constexpr Vec3<T> operator+(const Vec3<T>& left, T right)
{
    return { left.x + right, left.y + right, left.z + right };
}

template<typename T>
constexpr Vec3<T> operator-(const Vec3<T>& left, const Vec3<T>& right)
{
    return { left.x - right.x, left.y - right.y, left.z - right.z };
}

template<typename T>
constexpr Vec3<T> operator*(const Vec3<T>& left, T right)
{
    return { left.x * right, left.y * right, left.z * right };
}

template<typename T>
constexpr Vec3<T> operator-(const Vec3<T>& left, T right)
{
    return { left.x - right, left.y - right, left.z - right };
}

template<typename T>
constexpr Vec3<T> operator*(T left, const Vec3<T>& right)
{
    return { left.x * right, left.y * right, left.z * right };
}

template<typename T>
constexpr Vec3<T>& operator*=(Vec3<T>& left, T right)
{
    left.x *= right;
    left.y *= right;
    left.z *= right;

    return left;
}

template<typename T>
constexpr Vec3<T> operator/(const Vec3<T>& left, T right)
{
    return { left.x / right, left.y / right, left.z / right };
}

template<typename T>
constexpr Vec3<T>& operator/=(Vec3<T>& left, T right)
{
    left.x /= right;
    left.y /= right;
    left.z /= right;

    return left;
}

template<typename T>
constexpr Vec3<T>& operator/=(Vec3<T>& left, const Vec3<T>& right)
{
    left.x /= right.x;
    left.y /= right.y;
    left.z /= right.z;

    return left;
}

template<typename T>
constexpr bool operator==(const Vec3<T>& left, const Vec3<T>& right)
{
    return left.x == right.x && left.y == right.y && left.z == right.z;
}

// TODO: add spaceship operator

using Vec3f = Vec3<double>;
using Vec3i = Vec3<int>;
using Vec3u = Vec3<unsigned int>;

HRY_NS_END

template<typename T>
struct fmt::formatter<hry::Vec3<T>>
{
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(hry::Vec3<T> const& vec, FormatContext& ctx)
    {
        return fmt::format_to(ctx.out(), "{{{},{},{}}}", vec.x, vec.y, vec.z);
    }
};
