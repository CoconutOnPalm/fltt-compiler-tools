#pragma once

#include <format>
#include <vector>
#include <array>


// clang already has them defined
#ifndef __clang__


// std::vector
template <typename T>
struct std::formatter<std::vector<T>> : std::formatter<std::string>
{
	auto format(const std::vector<T>& vector, std::format_context& ctx) const
	{
		std::string elements;
		for (const auto& itr : vector)
			elements += std::format("{}, ", itr);

		if (vector.size() > 0)
		{
			elements.pop_back();
			elements.pop_back();
		}

		return std::formatter<std::string>::format(std::format("[{}]", elements), ctx);
	}
};


// std::array
template <typename T, size_t _size>
struct std::formatter<std::array<T, _size>> : std::formatter<std::string>
{
	auto format(const std::array<T, _size>& array, std::format_context& ctx) const
	{
		std::string elements;
		for (const auto& itr : array)
			elements += std::format("{}, ", itr);

		if (array.size() > 0)
		{
			elements.pop_back();
			elements.pop_back();
		}

		return std::formatter<std::string>::format(std::format("[{}]", elements), ctx);
	}
};


// std::pair
template <typename T1, typename T2>
struct std::formatter<std::pair<T1, T2>> : std::formatter<std::string>
{
	auto format(const std::pair<T1, T2>& pair, std::format_context& ctx) const
	{
		return std::formatter<std::string>::format(std::format("({}, {})", pair.first, pair.second), ctx);
	}
};

#endif