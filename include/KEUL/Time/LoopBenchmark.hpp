#pragma once

#include "Benchmark.hpp"


namespace ke
{
	/**
	 * @brief Benchmark class for handling in-loop benchmarking.
	 */
	class LoopBenchmark
	{
	private:


		std::string m_name;
		Benchmark m_clock;

		size_t m_iteration;
		std::pair<double, double> m_avarage;


	public:

		LoopBenchmark()
			: m_name("LoopBenchmark"), m_iteration(0), m_avarage(0, 0)
		{}

		/**
		 * @param name	Name of the instance. Useful for printing results.
		 */
		LoopBenchmark(const std::string& name, const TimeUnit time_unit)
			: m_name(name), m_iteration(0), m_avarage(0, 0), m_clock(time_unit)
		{}


		~LoopBenchmark() = default;

		/**
		 * @brief Use at the beggining of a block in loop. Starts single interation benchmark.
		 */
		void startIteration()
		{
			m_clock.start();
		}

		/**
		 * @brief Use after startIteration(). Ends single interation benchmark.
		 *
		 * @param print		true -> the results are printed (in microseconds)
		 * @return Elapsed time (in microseconds)
		 */
		long double endIteration(bool print = false)
		{
			auto val = m_clock.stop(false);

			if (print)
			{
				std::println("{0}: {1} -> {2}", m_name, m_iteration, val);
			}

			// arithmetic avarage
			m_avarage.second = (m_avarage.first * m_avarage.second + val) / (m_avarage.first + 1);
			m_avarage.first++;

			m_iteration++;

			return val;
		}

		/**
		 * Stops the whole benchmark and returns avarage iteration time.
		 *
		 * @param print If true, prints avarage iteration time.
		 * @return Avarage iteration time
		 */
		long double stop(bool print = false)
		{
			m_clock.stop(false); // just in case

			if (print)
				std::println("{0}: \t avg time = {1}", m_name, m_avarage.second);

			m_iteration = 0;

			return m_avarage.second;
		}
	};

} // namespace ke
