// Original code: https://github.com/SFML/SFML
// This is a part of SFML under https://github.com/SFML/SFML/blob/master/license.md

#pragma once

namespace hry
{

template<typename T>
struct Vec2
{
	T x;
	T y;
};

template <typename T>
constexpr Vec2<T> operator -(const Vec2<T>& right)
{
	return Vec2<T>(-right.x, -right.y);
}

template <typename T>
constexpr Vec2<T>& operator +=(Vec2<T>& left, const Vec2<T>& right)
{
	left.x += right.x;
	left.y += right.y;

	return left;
}

template <typename T>
constexpr Vec2<T>& operator -=(Vec2<T>& left, const Vec2<T>& right)
{
	left.x -= right.x;
	left.y -= right.y;

	return left;
}

template <typename T>
constexpr Vec2<T> operator +(const Vec2<T>& left, const Vec2<T>& right)
{
	return Vec2<T>(left.x + right.x, left.y + right.y);
}

template <typename T>
constexpr Vec2<T> operator -(const Vec2<T>& left, const Vec2<T>& right)
{
	return Vec2<T>(left.x - right.x, left.y - right.y);
}

template <typename T>
constexpr Vec2<T> operator *(const Vec2<T>& left, T right)
{
	return Vec2<T>(left.x * right, left.y * right);
}

template <typename T>
constexpr Vec2<T> operator *(T left, const Vec2<T>& right)
{
	return Vec2<T>(left.x * right, left.y * right);
}

template <typename T>
constexpr Vec2<T>& operator *=(Vec2<T>& left, T right)
{
	left.x *= right;
	left.y *= right;

	return left;
}

template <typename T>
constexpr Vec2<T> operator /(const Vec2<T>& left, T right)
{
	return Vec2<T>(left.x / right, left.y / right);
}

template <typename T>
constexpr Vec2<T>& operator /=(Vec2<T>& left, T right)
{
	left.x /= right;
	left.y /= right;

	return left;
}

using Vec2f = Vec2<float>;
using Vec2i = Vec2<int>;
using Vec2u = Vec2<unsigned int>;

}
