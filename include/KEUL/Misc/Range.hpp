#pragma once

#include "../String/StringLib.hpp"

namespace ke
{
	namespace policies
	{
		template <typename T> requires std::is_fundamental_v<T>
		struct OpenRangePolicy
		{
			static bool contains(T var, T a, T b)
			{
				return var > a && var < b;
			}

			static bool isDegenerate(T a, T b)
			{
				return a >= b;
			}

			static std::string toString(T a, T b)
			{
				// convertible to string asserted by concept
				return '(' + ke::toString(a) + ", " + ke::toString(b) + ')';
			}
		};

		template <typename T> requires std::is_fundamental_v<T>
		struct ClosedRangePolicy
		{
			static bool contains(T var, T a, T b)
			{
				return var >= a && var <= b;
			}

			static bool isDegenerate(T a, T b)
			{
				return a > b;
			}

			static std::string toString(T a, T b)
			{
				// convertible to string asserted by concept
				return '[' + ke::toString(a) + ", " + ke::toString(b) + ']';
			}
		};

		template <typename T> requires std::is_fundamental_v<T>
		struct HalfOpenRangePolicy
		{
			static bool contains(T var, T a, T b)
			{
				return var >= a && var < b;
			}

			static bool isDegenerate(T a, T b)
			{
				return a >= b;
			}

			static std::string toString(T a, T b)
			{
				// convertible to string asserted by concept
				return '[' + ke::toString(a) + ", " + ke::toString(b) + ')';
			}
		};
	}

	// internal compiler error whem using concept -> using unchecked template instead
	template <typename T, template <typename> class RangeType>
	concept _RangeTypePolicyConcept = requires(T var, T a, T b)
	{
		{ RangeType<T>::inRange(var, a, b) } -> std::convertible_to<bool>;
		{ RangeType<T>::isDegenerate(a, b) } -> std::convertible_to<bool>;
		{ RangeType<T>::toString(a, b) } -> std::convertible_to<std::string>;
	};

	/**
	 * @brief Representation of mathematical range.
	 * 
	 * @tparam T				type of range - must be fundamental
	 * @tparam RangeTypePolicy	type of range policy: OpenRangePolicy (), ClosedRangePolicy [], HalfOpenRangePolicy [)
	 */
	template <typename T, template <typename> class RangeTypePolicy>
		//requires std::is_fundamental_v<T>&& _RangeTypePolicyConcept<T, RangeTypePolicy>
	class Range
	{
	public:

		T a; /// lower bound
		T b; /// upper bound


		// placeholder until Microsoft fixes the compiler
		static_assert(std::is_fundamental_v<T>, "Range type must be fundamental");

		/**
		 * @brief Default constructor - creates (0, 0) range.
		 * 
		 */
		Range() : a(0), b(0) {}

		/**
		 * @brief Constructor - creates (lowerb, upperb) range.
		 *
		 * @param lowerb	lower bound
		 * @param upperb	upper bound
		 */
		Range(T lowerb, T upperb) : a(lowerb), b(upperb) {}

		/**
		 * @brief Copies values from other range, but ignores the policy.
		 *
		 * @param other		other range: can also be other type of range, but must have the same type T
		 */
		template <typename U, template <typename> class RTP>
			requires std::is_fundamental_v<T> // placeholder until Microsoft fixes the compiler
			//requires std::is_fundamental_v<T>&& _RangeTypePolicyConcept<T, RangeTypePolicy>
		Range(const Range<U, RTP>& other) : a(other.a), b(other.b) {}

		template <typename U, template <typename> class RTP>
			requires std::is_fundamental_v<T> // placeholder until Microsoft fixes the compiler 
			//requires std::is_fundamental_v<T>&& _RangeTypePolicyConcept<T, RangeTypePolicy>
		Range& operator=(const Range<U, RTP>& other)
		{
			a = static_cast<T>(other.a);
			b = static_cast<T>(other.b);
			return *this;
		}

		/**
		 * @brief compares values of two ranges, ignores the policy
		 * 
		 * @param right
		 * @return 
		 */
		bool operator==(const Range& right)
		{
			return a == right.a && b == right.b;
		}

		/**
		 * @brief compares values of two ranges, ignores the policy
		 * 
		 * @param right
		 * @return 
		 */
		bool operator!=(const Range& right)
		{
			return !(*this == right);
		}

		/**
		 * @brief	Checks if variable is within range's bounds.
		 *			Vtype must be comparable with range's type
		 *
		 * @param var	checked varaible: must cast into T type
		 * @return		true -> in range   |   false -> not in range
		 */
		bool contains(T var) const
		{
			return RangeTypePolicy<T>::contains(var, a, b);
		}

		/**
		 * @brief Checks if range is empty (range is degenerate)
		 *
		 * @return
		 */
		bool isDegenerate() const
		{
			return RangeTypePolicy<T>::isDegenerate(a, b);
		}

		/**
		 * @brief Converts range to string in appropriate form.
		 * 
		 * @return example: "(1, 10)", "[1, 10]", or any other form defined by policy
		 */
		std::string toString() const
		{
			return RangeTypePolicy<T>::toString(a, b);
		}

	};


	/**
	 * @brief Representation of mathematical closed range [a, b].
	 */
	template <typename T>
	using ClosedRange = Range<T, policies::ClosedRangePolicy>;

	/**
	 * @brief Representation of mathematical open range (a, b).
	 */
	template <typename T>
	using OpenRange = Range<T, policies::OpenRangePolicy>;

	/**
	 * @brief Representation of mathematical half-open range [a, b).
	 */
	template <typename T>
	using HalfOpenRange = Range<T, policies::HalfOpenRangePolicy>;


	/**
	 * @brief	Checks if value is within range.
	 *
	 * @param rng	provides .contains() method used to check if value is in range
	 * @param val	Checked value - must be castable to rngType
	 * @return		true -> in range   |   false -> not in range
	 */
	template <typename T, template <typename> class RangeType>
		requires std::is_fundamental_v<T> // placeholder until Microsoft fixes the compiler
		//requires std::is_fundamental_v<T> && _RangeTypePolicyConcept<T, RangeType>
	inline bool inRange(const Range<T, RangeType>& rng, T val)
	{
		return rng.contains(val);
	}


	template <typename T, typename U1, typename U2>
		requires std::is_fundamental_v<T> && std::is_fundamental_v<U1> && std::is_fundamental_v<U2>
	inline bool restrainVariable(T& var, U1 a, U2 b)
	{
		// check for degenerate range
		if (a >= b)
			return false;

		if (var < a)
		{
			var = static_cast<T>(a);
			return true;
		}
		if (var > b)
		{
			var = static_cast<T>(b);
			return true;
		}
		return false;
	}



	template <typename T>
		requires std::is_fundamental_v<T> // placeholder until Microsoft fixes the compiler
	inline bool restrainVariable(T& var, const ClosedRange<T>& rng)
	{
		if (rng.isDegenerate())
			return false;

		if (var < rng.a)
		{
			var = rng.a;
			return true;
		}
		if (var > rng.b)
		{
			var = rng.b;
			return true;
		}
		return false;
	}


} // namespace ke


template <typename T, template <typename> class RangeTypePolicy, typename CharT>
struct std::formatter<ke::Range<T, RangeTypePolicy>, CharT> : std::formatter<std::string>
{
	constexpr auto parse(std::basic_format_parse_context<CharT>& ctx)
	{
		return ctx.begin();
	}

	template <typename FormatContext>
	auto format(const ke::Range<T, RangeTypePolicy>& range, FormatContext& ctx) const
	{
		return std::format_to(ctx.out(), "{}", range.toString());
	}
};
