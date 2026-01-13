#pragma once

#include <random>
#include <vector>
#include "Range.hpp"


namespace ke
{
	/**
	 * @brief Class for easy and simple random number generation.
	 */
	class Random
	{
	public:

		
        /**
		* @brief Default constructor for the Random class. Generates a random seed using std::random_device and std::default_random_engine.
        */
        Random() { m_engine.seed(m_seed_generator()); };

        /**
        * @brief Constructor for the Random class with a custom seed. Uses std::default_random_engine.
        *
        * @param seed The seed value for the random number generator.
        */
        Random(unsigned int seed) { m_engine.seed(seed); };

        /**
        * @brief Copies std::default_random_engine from the other generator.
        *
        * @param other The Random object to be copied.
        */
        Random(const Random& other) { m_engine = other.m_engine; };

        /**
        * @brief Copies std::default_random_engine from the other generator.
        *
        * @param other The Random object to be assigned.
        * @return A reference to the assigned Random object.
        */
        Random& operator=(const Random& other) 
		{ 
			m_engine = other.m_engine; 
			return *this; 
		};

		~Random() = default;



		/**
		 * @brief Returns random value within the [lowerb, upperb] range.
		 *
		 * @details
		 * This function utilizes std::uniform_int_distribution<T> with std::default_random_engine to produce output
		 * 
		 * @param lowerb	lower bound
		 * @param upperb	upper bound
		 * @return Random value within the [lowerb, upperb] range.
		 */
		template <typename T> requires std::is_integral_v<T>
		T Value(T lowerb, T upperb)
		{
			std::uniform_int_distribution<T> integer_distributor(lowerb, upperb);
			return integer_distributor(m_engine);
		}


		// TODO: make it template<typname T, RangeTypePolicyConcept _RngType> when compiler is fixed

		/**
		 * @brief Returns random value within the custom range.
		 * 
		 * @details
		 * This function utilizes std::uniform_int_distribution<T> with std::default_random_engine to produce output
		 * 
		 * @param range		Range<T, _RngType> object
		 * @return Random value within the custom range.
		 */
		template <typename T, template <typename> class _RngType> 
		requires std::is_integral_v<T> // std::is_fundamental_v<T> is implied by std::is_integral_v<T>
		T Value(const Range<T, _RngType>& range)
		{
			// optimize for existing range types
			if constexpr (std::is_same_v<_RngType<T>, policies::ClosedRangePolicy<T>>)
				return Value(range.a, range.b);
			if constexpr (std::is_same_v<_RngType<T>, policies::OpenRangePolicy<T>>)
				return Value(range.a + 1, range.b - 1);
			if constexpr (std::is_same_v<_RngType<T>, policies::HalfOpenRangePolicy<T>>)
				return Value(range.a, range.b - 1);

			T a = range.a;
			T b = range.b;

			if (!range.contains(a))
				a = range.a + 1;
			if (!range.contains(b))
				b = range.b - 1;

			return Value(a, b);
		}


		/**
		 * @brief Returns random integer within [lowerb, upperb] range.
		 *
		 * @details
		 * This function utilizes std::uniform_int_distribution<int> with std::default_random_engine to produce output
		 * 
		 * @param lowerb	lower bound
		 * @param upperb	upper bound
		 * @return Random integer within [lowerb, upperb] range.
		 */
		int32_t Int32(int32_t lowerb, int32_t upperb)
		{
			std::uniform_int_distribution<int32_t> integer_distributor(lowerb, upperb);
			return integer_distributor(m_engine);
		}


		/**
		 * @brief Returns random int32_t integer within the specified range.
		 *
		 * @details
		 * This function utilizes std::uniform_int_distribution<int32_t> with std::default_random_engine to produce output
		 * 
		 * @param range		Range<int32_t, _RngType> object
		 * @return Random 	int32_t integer within the specified range.
		 */
		template <template <typename rngType> class _RngType>
		int32_t Int32(const Range<int32_t, _RngType>& range)
		{
			return Value<int32_t>(range);
		}


		/**
		 * @brief Returns random 64-bit integer within [lowerb, upperb] range.
		 *
		 * @details
		 * This function utilizes std::uniform_int_distribution<int64_t> with std::default_random_engine to produce output
		 * 
		 * @param lowerb	lower bound
		 * @param upperb	upper bound
		 * @return Random 64-bit integer within [lowerb, upperb] range.
		 */
		int64_t Int64(int64_t lowerb, int64_t upperb)
		{
			std::uniform_int_distribution<int64_t> integer_distributor(lowerb, upperb);
			return integer_distributor(m_engine);
		}


		/**
		 * @brief Returns random int64_t integer within the specified range.
		 *
		 * @details
		 * This function utilizes std::uniform_int_distribution<int64_t> with std::default_random_engine to produce output
		 * 
		 * @param range		Range<int64_t, _RngType> object
		 * @return Random int64_t integer within the specified range.
		 */
		template <template <typename rngType> class _RngType>
		int64_t Int64(const Range<int64_t, _RngType>& range)
		{
			return Value<int64_t>(range);
		}


		/**
		 * @brief Returns random uint64_t integer within [lowerb, upperb] range.
		 *
		 * @details
		 * This function utilizes std::uniform_int_distribution<uint64_t> with std::default_random_engine to produce output
		 * 
		 * @param lowerb	lower bound
		 * @param upperb	upper bound
		 * @return Random uint64_t integer within [lowerb, upperb] range.
		 */
		uint64_t UInt64(uint64_t lowerb, uint64_t upperb)
		{
			std::uniform_int_distribution<uint64_t> integer_distributor(lowerb, upperb);
			return integer_distributor(m_engine);
		}


		/**
		 * @brief Returns random uint64_t integer within the specified range.
		 *
		 * @details
		 * This function utilizes std::uniform_int_distribution<uint64_t> with std::default_random_engine to produce output
		 *
		 * @param range		Range<uint64_t, _RngType> object
		 * @return Random uint64_t integer within the specified range.
		 */
		template <template <typename rngType> class _RngType>
		uint64_t UInt64(const Range<uint64_t, _RngType>& range)
		{
			return Value<uint64_t>(range);
		}



		/**
		 * @brief Returns random character within [lowerb, upperb] range.
		 *
		 * @details
		 * This function utilizes std::uniform_int_distribution<short> with std::default_random_engine to produce output
		 * Example: ke::Random::Char(a, z) gives random character from a to z
		 * 
		 * @param lowerb	lower bound
		 * @param upperb	upper bound
		 * @return Random character within [lowerb, upperb] range.
		*/
		char Char(char lowerb, char upperb)
		{
			std::uniform_int_distribution<short> integer_distributor(static_cast<short>(lowerb), static_cast<short>(upperb));
			return static_cast<char>(integer_distributor(m_engine));
		}


		/**
		 * @brief Returns random character within the specified range.
		 *
		 * @details
		 * This function utilizes std::uniform_int_distribution<short> with std::default_random_engine to produce output
		 * Example: ke::Random::Char(a, z) gives random character from a to z
		 *
		 * @param range		Range<char, _RngType> object
		 * @return Random character within the specified range.
		 */
		template <template <typename rngType> class _RngType>
		char Char(const Range<char, _RngType>& range)
		{
			// std::uniform_int_distribution<char> is not allowed, so we use short instead

			// optimize for existing range types
			if constexpr (std::is_same_v<_RngType<char>, policies::ClosedRangePolicy<char>>)
				return Char(range.a, range.b);
			if constexpr (std::is_same_v<_RngType<char>, policies::OpenRangePolicy<char>>)
				return Char(range.a + 1, range.b - 1);
			if constexpr (std::is_same_v<_RngType<char>, policies::HalfOpenRangePolicy<char>>)
				return Char(range.a, range.b - 1);

			char a = range.a;
			char b = range.b;

			if (!range.contains(a))
				a = range.a + 1;
			if (!range.contains(b))
				b = range.b - 1;

			return Char(a, b);
		}


		/**
		 * @brief Returns random double within [lowerb, upperb] range.
		 *
		 * @details
		 * This function utilizes std::uniform_real_distribution<double> with std::default_random_engine to produce output
		 * 
		 * @param lowerb	lower bound
		 * @param upperb	upper bound
		 * @return Random double within [lowerb, upperb] range.
		 */
		double Double(double lowerb, double upperb)
		{
			std::uniform_real_distribution<double> double_distributor(lowerb, upperb);
			return double_distributor(m_engine);
		}


		/**
		 * @brief Returns random double within the specified range.
		 *
		 * @details
		 * This function utilizes std::uniform_real_distribution<double> with std::default_random_engine to produce output
		 *
		 * @param range		Range<double, _RngType> object
		 * @return Random double within the specified range.
		 */
		template <template <typename rngType> class _RngType>
		double Double(const Range<double, _RngType>& range)
		{
			constexpr double eps = std::numeric_limits<double>().epsilon();

			// optimize for existing range types
			if constexpr (std::is_same_v<_RngType<double>, policies::ClosedRangePolicy<double>>)
				return Double(range.a, range.b);
			if constexpr (std::is_same_v<_RngType<double>, policies::OpenRangePolicy<double>>)
				return Double(range.a + eps, range.b - eps);
			if constexpr (std::is_same_v<_RngType<double>, policies::HalfOpenRangePolicy<double>>)
				return Double(range.a, range.b - eps);

			double a = range.a;
			double b = range.b;

			if (!range.contains(a))
				a = range.a + eps;
			if (!range.contains(b))
				b = range.b - eps;

			return Double(a, b);
		}


		/**
		 * @brief Returns random float within [lowerb, upperb] range.
		 *
		 * @details
		 * This function utilizes std::uniform_real_distribution<float> with std::default_random_engine to produce output
		 *
		 * @param lowerb	lower bound
		 * @param upperb	upper bound
		 * @return Random double within [lowerb, upperb] range.
		 */
		float Float(float lowerb, float upperb)
		{
			std::uniform_real_distribution<float> double_distributor(lowerb, upperb);
			return double_distributor(m_engine);
		}


		/**
		 * @brief Returns random float within the specified range.
		 *
		 * @details
		 * This function utilizes std::uniform_real_distribution<float> with std::default_random_engine to produce output
		 *
		 * @param range		Range<float, _RngType> object
		 * @return Random float within the specified range.
		 */
		template <template <typename rngType> class _RngType>
		float Float(const Range<float, _RngType>& range)
		{
			constexpr float eps = std::numeric_limits<float>().epsilon();

			// optimize for existing range types
			if constexpr (std::is_same_v<_RngType<float>, policies::ClosedRangePolicy<float>>)
				return Float(range.a, range.b);
			if constexpr (std::is_same_v<_RngType<float>, policies::OpenRangePolicy<float>>)
				return Float(range.a + eps, range.b - eps);
			if constexpr (std::is_same_v<_RngType<float>, policies::HalfOpenRangePolicy<float>>)
				return Float(range.a, range.b - eps);

			float a = range.a;
			float b = range.b;

			if (!range.contains(a))
				a = range.a + eps;
			if (!range.contains(b))
				b = range.b - eps;

			return Float(a, b);
		}


		/**
		 * @brief Genrates string of random characters within defined range(s).
		 * 
		 * @param lenght		Length of the string
		 * @param ...ranges		ke::ClosedRange<char>. Every character is generated from 
		 * @return 
		 */
		template<typename... ke_ranges>
		std::string String(size_t lenght, ke_ranges... ranges)
		{
			std::vector<ClosedRange<char>> pairs = { static_cast<ClosedRange<char>>(ranges)... };

			// if no arguments provided, generate text from all printable characters (from ! to ~)
			if (pairs.empty())
				pairs.emplace_back(ClosedRange('!', '~'));

			std::string output;
			output.reserve(lenght);

			for (size_t i = 0; i < lenght; i++)
			{
				int selected_range = Int32(0, static_cast<int>(pairs.size()) - 1);
				output += Char(pairs[selected_range].a, pairs[selected_range].b);
			}

			return output;
		}


		/**
 		* @brief Returns true by chance, base on <percentage>. See details to know how it works
 		* 
 		* @details
 		* draws a number between 0 and 1 (treat it as %) and if value is below the <percent>, returns true.
 		* Example:
 		*	Imagine you want to toss a coin. Then, you can simulate that by calling ke::Chance(0.5), so it returns true 50% of the time
 		* 
 		* @param percent
 		* @return 
 		*/
		bool Chance(float percent)
		{
			return ClosedRange(0.f, percent).contains(Float(0, 1));
		}


		/**
		 * @brief Sets custom seed for random number generator
		 * 
		 * @param seed
		 */
		void SetSeed(unsigned int seed)
		{
			m_engine.seed(seed);
		}


	private:

		// NOTE: creating new distrubutor every time is somehow more efficient than creating it once and storing it as static variable

		std::random_device m_seed_generator;
		std::default_random_engine m_engine;
	};

} // namespace ke
