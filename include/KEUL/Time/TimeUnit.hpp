#pragma once

#include <print>
#include <iostream>
#include <sstream>
#include <chrono>
#include <iomanip> 

#ifdef _MSC_VER
#pragma execution_character_set( "utf-8" )
#endif



namespace ke
{
	enum class TimeUnit
	{
		nanoseconds,
		microseconds,
		miliseconds,
		seconds,
		minutes
	};


	namespace _internal
	{
		static inline std::string _timeUnitToString(const TimeUnit timeunit)
		{
			const static std::map<ke::TimeUnit, std::string> s_timeUnitToString = {
				{ke::TimeUnit::nanoseconds, "ns"},
				{ke::TimeUnit::microseconds, "\u00B5s"}, // µs
				{ke::TimeUnit::miliseconds, "ms"},
				{ke::TimeUnit::seconds, "s"},
				{ke::TimeUnit::minutes, "min"},
			};

			return s_timeUnitToString.at(timeunit);
		}
	}
}


template <>
struct std::formatter<ke::TimeUnit> : std::formatter<std::string>
{
	template <typename FormatContext>
	auto format(const ke::TimeUnit time_unit, FormatContext& ctx) const
	{
		return std::formatter<std::string>::format(std::format("{}", ke::_internal::_timeUnitToString(time_unit)), ctx);
	}
};