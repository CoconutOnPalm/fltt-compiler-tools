#pragma once

#include <string>
#include <functional>
#include <vector>


namespace ke::test::_internal
{
	template <typename T, typename U>
	concept _EqualComparableConcept = requires(T a, U b)
	{
		{ a == b } -> std::convertible_to<bool>;
	};

	template <typename T, typename U, typename FloatingPoint>
	concept _SimilarComparableConcept = requires (T a, U b, FloatingPoint epsilon)
	{
		{ std::abs(a - b) <= epsilon } -> std::convertible_to<bool>;
	}&& std::is_arithmetic_v<T>&& std::is_arithmetic_v<U>&& std::is_floating_point_v<FloatingPoint>;


	enum class TestResult
	{
		Success,
		Failure,

		None
	};


	struct TestData
	{
		std::string test_type;
		std::string left_expression = "???";
		std::string left_value = "???";
		std::string right_expression = "???";
		std::string right_value = "???";

		size_t index;
		uint64_t line;
		std::string file;

		TestResult result = TestResult::None;
	};

	struct TestSummary
	{
		TestResult result = TestResult::None;
		double total_time = 0;

		size_t success_count = 0;
		size_t failure_count = 0;
		size_t interrupted_count = 0;

		bool aborted = false;
		std::string abort_reason;
	};
}
