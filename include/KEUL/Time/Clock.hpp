#pragma once

#include <print>
#include <iostream>
#include <sstream>
#include <chrono>
#include <iomanip> 

#ifdef _MSC_VER
#pragma execution_character_set( "utf-8" )
#endif

#include "TimeUnit.hpp"




namespace ke
{


	/**
	 * @brief Real-time automated clock based on std::chromo::high_resolution_clock.
	 */
	class Clock
	{
	public:


	private:

		TimeUnit m_unit;

		std::chrono::nanoseconds m_total_time;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_start, m_stop;

	public:

		/**
		 * @brief Default constructor - does not start the clock.
		 *
		 * @param unit	time unit (ke::Clock::TimeUnit)
		 */
		Clock(TimeUnit unit = TimeUnit::microseconds)
			: m_unit(unit), m_total_time()
		{
		}

		/**
		 * @brief Starts the clock.
		 */
		void start()
		{
			m_start = std::chrono::high_resolution_clock::now();
		}

		/**
		 * @brief Stops the clock.
		 *
		 * @return Elapsed time
		 */
		long double stop()
		{
			m_stop = std::chrono::high_resolution_clock::now();
			m_total_time = m_stop - m_start;

			return _getDuration(m_unit, m_total_time);
		}

		/**
		 * @brief Returns .
		 *
		 * @param unit	time format (ke::Clock::TimeUnit)
		 * @return
		 */
		long double getLastMeasurement(const TimeUnit unit = TimeUnit::microseconds) const
		{
			return _getDuration(unit, m_total_time);
		}

		/**
		 * @brief Returns string representation ("<time> <unit>") of latest measurement. See details for additional info
		 *
		 * @details
		 * String in "<time> <unit>" format, where
		 *	<time> is a floating point converted to std::string with specified precision
		 *	<unit> is suffix based on specified format
		 *
		 * Example:
		 *	let time = 21.37420 miliseconds, precision = 2
		 *	the output string will be "21.37 ms"
		 *
		 * @param unit	time format (ke::Clock::TimeUnit)
		 * @return ("<time> <unit>"). See details for additional info
		 */
		std::string toString() const
		{
			long double duration = _getDuration(m_unit, m_total_time);
			return std::format("{:.3} {}", duration, m_unit);
		}


	private:

		long double _getDuration(const TimeUnit unit, const std::chrono::nanoseconds total_ns) const
		{
			// switch-case probably better than std::map in this case
			switch (unit)
			{
			case TimeUnit::nanoseconds:
				return m_total_time.count();
				break;
			case TimeUnit::microseconds:
				return std::chrono::duration_cast<std::chrono::microseconds>(total_ns).count();
				break;
			case TimeUnit::miliseconds:
				return std::chrono::duration_cast<std::chrono::milliseconds>(total_ns).count();
				break;
			case TimeUnit::seconds:
				return std::chrono::duration_cast<std::chrono::seconds>(total_ns).count();
				break;
			case TimeUnit::minutes:
				return std::chrono::duration_cast<std::chrono::minutes>(total_ns).count();
				break;
			default:
				throw std::invalid_argument("TimeUnit is incorrect");
				break;
			}

			return total_ns.count();
		}
	};

} // namespace ke


template <>
struct std::formatter<ke::Clock> : std::formatter<std::string>
{
	template <typename FormatContext>
	auto format(const ke::Clock clock, FormatContext& ctx) const
	{
		return std::formatter<std::string>::format(clock.toString(), ctx);
	}
};