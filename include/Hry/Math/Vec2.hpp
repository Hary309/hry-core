/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include "fmt/core.h"

namespace hry
{
/**
 * @name 2D Vector type
 */
template<typename T>
struct Vec2
{
    T x;
    T y;

    Vec2() = default;
    Vec2(T x, T y)
        : x(x), y(y) {}
    explicit Vec2(T value)
        : x(value), y(value) {}
};

template<typename T>
constexpr Vec2<T> operator-(const Vec2<T>& right)
{
    return { -right.x, -right.y };
}

template<typename T>
constexpr Vec2<T>& operator+=(Vec2<T>& left, const Vec2<T>& right)
{
    left.x += right.x;
    left.y += right.y;

    return left;
}

template<typename T>
constexpr Vec2<T>& operator-=(Vec2<T>& left, const Vec2<T>& right)
{
    left.x -= right.x;
    left.y -= right.y;

    return left;
}

template<typename T>
constexpr Vec2<T> operator+(const Vec2<T>& left, const Vec2<T>& right)
{
    return { left.x + right.x, left.y + right.y };
}

template<typename T>
constexpr Vec2<T> operator+(const Vec2<T>& left, T right)
{
    return { left.x + right, left.y + right };
}

template<typename T>
constexpr Vec2<T> operator-(const Vec2<T>& left, const Vec2<T>& right)
{
    return { left.x - right.x, left.y - right.y };
}

template<typename T>
constexpr Vec2<T> operator-(const Vec2<T>& left, T right)
{
    return { left.x - right, left.y - right };
}

template<typename T>
constexpr Vec2<T> operator*(const Vec2<T>& left, T right)
{
    return { left.x * right, left.y * right };
}

template<typename T>
constexpr Vec2<T> operator*(T left, const Vec2<T>& right)
{
    return { left.x * right, left.y * right };
}

template<typename T>
constexpr Vec2<T>& operator*=(Vec2<T>& left, T right)
{
    left.x *= right;
    left.y *= right;

    return left;
}

template<typename T>
constexpr Vec2<T> operator/(const Vec2<T>& left, T right)
{
    return { left.x / right, left.y / right };
}

template<typename T>
constexpr Vec2<T>& operator/=(Vec2<T>& left, T right)
{
    left.x /= right;
    left.y /= right;

    return left;
}

template<typename T>
constexpr Vec2<T>& operator/=(Vec2<T>& left, const Vec2<T>& right)
{
    left.x /= right.x;
    left.y /= right.y;

    return left;
}

template<typename T>
constexpr bool operator==(const Vec2<T>& left, const Vec2<T>& right)
{
    return left.x == right.x && left.y == right.y;
}

// TODO: add spaceship operator

using Vec2f = Vec2<double>;
using Vec2i = Vec2<int>;
using Vec2u = Vec2<unsigned int>;
}

namespace fmt
{
template<typename T>
struct formatter<hry::Vec2<T>>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx)
    {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(hry::Vec2<T> const& vec, FormatContext& ctx)
    {
        return format_to(ctx.out(), "{{{}, {}}}", vec.x, vec.y);
    }
};
} // namespace fmt
