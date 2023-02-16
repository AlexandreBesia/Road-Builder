#pragma once

#include <cmath>

#include <SFML/System/Vector2.hpp>

namespace Math
{
	template <typename T = float>
	[[nodiscard]] constexpr T pi()
	{
		static_assert(std::is_floating_point_v<T>, "'pi()' only accepts floating point types");
		return static_cast<T>(3.1415926535897932L);
	}

	template <typename T>
	[[nodiscard]] constexpr T radians(T degrees)
	{
		static_assert(std::is_floating_point_v<T>, "'radians()' only accepts floating point types");
		return degrees * (pi<T>() / static_cast<T>(180.f));
	}

	template <typename T>
	[[nodiscard]] constexpr T degrees(T radians)
	{
		static_assert(std::is_floating_point_v<T>, "'degrees()' only accepts floating point types");
		return radians * (static_cast<T>(180.f) / pi<T>());
	}

	template <typename T>
	[[nodiscard]] T magnitude(const sf::Vector2<T>& vector)
	{
		return sqrt(vector.x * vector.x + vector.y * vector.y);
	}

	template <typename T>
	[[nodiscard]] T lerp(T start, T end, T factor)
	{
		return start + factor * (end - start);
	}

	template <typename T>
	[[nodiscard]] sf::Vector2<T> normalize(const sf::Vector2<T>& vector)
	{
		return vector / magnitude(vector);
	}

	template <typename T>
	[[nodiscard]] T dot(const sf::Vector2<T>& v1, const sf::Vector2<T>& v2)
	{
		return v1.x * v2.x + v1.y * v2.y;
	}

	template <typename T>
	[[nodiscard]] T angle(const sf::Vector2<T>& v1, const sf::Vector2<T>& v2)
	{
		return acos(dot(normalize(v1), normalize(v2)));
	}
}