#pragma once

#include <type_traits>

#include "../String/Format.hpp"
#include "../Time/Clock.hpp"
#include "TestDef.hpp"



namespace ke::test::_internal
{
	

	class TestBase
	{
	private:


		bool m_aborted = false;

		std::string m_name;
		std::vector<TestData> m_data;
		uint32_t m_test_index = 0;

		bool m_benchmark_enabled = true;
		::ke::TimeUnit m_time_unit = TimeUnit::miliseconds;

		TestSummary m_summary;



	public:
		virtual ~TestBase() = default;

	protected:

		TestBase(const std::string& name) : m_name(name), m_test_index(0)
			{}


		virtual void Run() = 0;


		void __ON_INTERRUPTION(const std::string& test_type, const std::string_view left_expr_string, const std::string_view right_expr_string, const std::string& file, uint32_t line)
		{
			TestData data { 
				.test_type = test_type, 
				.left_expression = std::string(left_expr_string), 
				.left_value = "exception thrown",
				.right_expression = std::string(right_expr_string),
				.right_value = "exception thrown",
				.index = m_test_index, 
				.line = line, 
				.file = file };

			m_test_index++;
			data.result = TestResult::Failure;
			m_data.push_back(data);
		}


		template <typename T, typename U> requires _EqualComparableConcept<T, U>
		void __ASSERT_EQUAL(const T& left, const U& right, const std::string_view left_expr_string, const std::string_view right_expr_string, const std::string_view file, uint32_t line)
		{
			if (m_aborted) return;

			TestData data{ .test_type = "ASSERT EQUAL", .index = m_test_index, .line = line, .file = std::string(file)};
			m_test_index++;

			if (left == right)
			{
				data.result = TestResult::Success;
			}
			else
			{
				data.result = TestResult::Failure;
				data.left_expression = left_expr_string;
				data.right_expression = right_expr_string;

				if constexpr (std::is_convertible_v<T, std::string> || std::is_fundamental_v<T> || std::is_convertible_v<T, std::string_view> || std::formattable<T, char>)
				{
					data.left_value = ke::format("{}", left);
					data.right_value = ke::format("{}", right);
				}
				else
				{
					data.left_value = "expression result not formattable";;
					data.right_value = "expression result not formattable";
				}
			}

			m_data.push_back(data);
		}


		template <typename T, typename U>
		void __ASSERT_NOT_EUQAL(const T& left, const U& right, const std::string_view left_expr_string, const std::string_view right_expr_string, const std::string_view file, uint32_t line)
		{
			if (m_aborted) return;

			TestData data{ .test_type = "ASSERT NOT EQUAL", .index = m_test_index, .line = line, .file = std::string(file) };
			m_test_index++;

			if (left != right)
			{
				data.result = TestResult::Success;
			}
			else
			{
				data.result = TestResult::Failure;
				data.left_expression = left_expr_string;
				data.right_expression = right_expr_string;

				if constexpr (std::is_convertible_v<T, std::string> || std::is_fundamental_v<T> || std::is_convertible_v<T, std::string_view> || std::formattable<T, char>)
				{
					data.left_value = ke::format("{}", left);
					data.right_value = ke::format("{}", right);
				}
				else
				{
					data.left_value = "expression result not formattable";
					data.right_value = "expression result not formattable";
				}
			}

			m_data.push_back(data);
		}


		template <typename T, typename U> requires _SimilarComparableConcept<T, U, float>
		void __ASSERT_SIMILAR(const T& left, const U& right, float epsilon, const std::string_view left_expr_string, const std::string_view right_expr_string, const std::string_view file, uint32_t line)
		{
			if (m_aborted) return;

			TestData data{ .test_type = "ASSERT SIMILAR (float)", .index = m_test_index, .line = line, .file = std::string(file) };
			m_test_index++;

			if (std::abs(left - right) < epsilon)
			{
				data.result = TestResult::Success;
			}
			else
			{
				data.result = TestResult::Failure;
				data.left_expression = left_expr_string;
				data.right_expression = right_expr_string;

				if constexpr (std::is_convertible_v<T, std::string> || std::is_fundamental_v<T> || std::is_convertible_v<T, std::string_view> || std::formattable<T, char>)
				{
					data.left_value = ke::format("{}", left);
					data.right_value = ke::format("{}", right);
				}
				else
				{
					data.left_value = "expression result not formattable";
					data.right_value = "expression result not formattable";
				}
			}

			m_data.push_back(data);
		}


		template <typename T, typename U> requires _SimilarComparableConcept<T, U, double>
		void __ASSERT_SIMILAR_DOUBLE(const T& left, const U& right, double epsilon, const std::string_view left_expr_string, const std::string_view right_expr_string, const std::string_view file, uint32_t line)
		{
			if (m_aborted) return;

			TestData data{ .test_type = "ASSERT SIMILAR (float)", .index = m_test_index, .line = line, .file = std::string(file) };
			m_test_index++;

			if (std::abs(left - right) < epsilon)
			{
				data.result = TestResult::Success;
			}
			else
			{
				data.result = TestResult::Failure;
				data.left_expression = left_expr_string;
				data.right_expression = right_expr_string;

				if constexpr (std::is_convertible_v<T, std::string> || std::is_fundamental_v<T> || std::is_convertible_v<T, std::string_view> || std::formattable<T, char>)
				{
					data.left_value = ke::format("{}", left);
					data.right_value = ke::format("{}", right);
				}
				else
				{
					data.left_value = "expression result not formattable";
					data.right_value = "expression result not formattable";
				}
			}

			m_data.push_back(data);
		}


		template <typename T, typename U> requires _SimilarComparableConcept<T, U, long double>
		void __ASSERT_NOT_SIMILAR(const T& left, const U& right, long double epsilon, const std::string_view left_expr_string, const std::string_view right_expr_string, const std::string_view file, uint32_t line)
		{
			if (m_aborted) return;

			TestData data{ .test_type = "ASSERT NOT SIMILAR", .index = m_test_index, .line = line, .file = std::string(file) };
			m_test_index++;

			if (std::abs(left - right) > epsilon)
			{
				data.result = TestResult::Success;
			}
			else
			{
				data.result = TestResult::Failure;
				data.left_expression = left_expr_string;
				data.right_expression = right_expr_string;

				if constexpr (std::is_convertible_v<T, std::string> || std::is_fundamental_v<T> || std::is_convertible_v<T, std::string_view> || std::formattable<T, char>)
				{
					data.left_value = ke::format("{}", left);
					data.right_value = ke::format("{}", right);
				}
				else
				{
					data.left_value = "expression result not formattable";
					data.right_value = "expression result not formattable";
				}
			}

			m_data.push_back(data);
		}


		template <typename T>
		void __ASSERT_TRUE(const T& expr, const std::string_view expr_string, const std::string_view file, uint32_t line)
		{
			if (m_aborted) return;

			TestData data{ .test_type = "ASSERT TRUE", .index = m_test_index, .line = line, .file = std::string(file) };
			m_test_index++;

			if (expr == true)
			{
				data.result = TestResult::Success;
			}
			else
			{
				data.result = TestResult::Failure;
				data.left_expression = expr_string;
				data.right_expression = "true";

				if constexpr (std::is_convertible_v<T, std::string> || std::is_fundamental_v<T> || std::is_convertible_v<T, std::string_view> || std::formattable<T, char>)
				{
					data.left_value = ke::format("{}", expr);
					data.right_value = "true";
				}
				else
				{
					data.left_value = "expression result not formattable";
					data.right_value = "true";
				}
			}

			m_data.push_back(data);
		}


		template <typename T>
		void __ASSERT_FALSE(const T& expr, const std::string_view expr_string, const std::string_view file, uint32_t line)
		{
			if (m_aborted) return;

			TestData data{ .test_type = "ASSERT FALSE", .index = m_test_index, .line = line, .file = std::string(file) };
			m_test_index++;

			if (expr == false)
			{
				data.result = TestResult::Success;
			}
			else
			{
				data.result = TestResult::Failure;
				data.left_expression = expr_string;
				data.right_expression = "false";

				if constexpr (std::is_convertible_v<T, std::string> || std::is_fundamental_v<T> || std::is_convertible_v<T, std::string_view> || std::formattable<T, char>)
				{
					data.left_value = ke::format("{}", expr);
					data.right_value = "false";
				}
				else
				{
					data.left_value = "expression result not formattable";
					data.right_value = "false";
				}
			}

			m_data.push_back(data);
		}



		template <typename T, typename ErrorType>
		void __ASSERT_ERROR(const std::expected<T, ErrorType>& expr, ErrorType error, std::string_view expr_string, const std::string_view file, uint32_t line)
		{
			if (m_aborted) return;

			TestData data{ .test_type = "ASSERT ERROR", .index = m_test_index, .line = line, .file = std::string(file) };
			m_test_index++;

			if (expr.has_value() == false && expr.error() == error)
			{
				data.result = TestResult::Success;
			}
			else
			{
				data.result = TestResult::Failure;
				data.left_expression = expr_string;
				data.right_expression = ::ke::toString(error);

				if constexpr 
					((std::is_convertible_v<T, std::string> || std::is_fundamental_v<T> || std::is_convertible_v<T, std::string_view> || std::formattable<T, char>) &&
					 (std::is_convertible_v<ErrorType, std::string> || std::is_fundamental_v<ErrorType> || std::is_convertible_v<ErrorType, std::string_view> || std::formattable<ErrorType, char>))
				{
					data.left_value = ke::format("{}", expr);
					data.right_value.clear();
				}
				else
				{
					data.left_value = "expression result not formattable";
				}
			}

			m_data.push_back(data);
		}


		template <typename T, typename ErrorType>
		void __ASSERT_NO_ERROR(const std::expected<T, ErrorType>& expr, std::string_view expr_string, const std::string_view file, uint32_t line)
		{
			if (m_aborted) return;

			TestData data{ .test_type = "ASSERT NO ERROR", .index = m_test_index, .line = line, .file = std::string(file) };
			m_test_index++;

			if (expr.has_value())
			{
				data.result = TestResult::Success;
			}
			else
			{
				data.result = TestResult::Failure;
				data.left_expression = expr_string;
				data.right_expression = "";

				if constexpr ((std::is_convertible_v<T, std::string> || std::is_fundamental_v<T> || std::is_convertible_v<T, std::string_view> || std::formattable<T, char>) &&
					(std::is_convertible_v<ErrorType, std::string> || std::is_fundamental_v<ErrorType> || std::is_convertible_v<ErrorType, std::string_view> || std::formattable<ErrorType, char>))
				{
					data.left_value = ke::format("{}", expr);
					data.right_value.clear();
				}
				else
				{
					data.left_value = "expression result not formattable";
				}
			}

			m_data.push_back(data);
		}


		void __ABORT_TESTS(std::string_view reason)
		{
			m_aborted = true;

			m_summary.aborted = true;
			m_summary.abort_reason = reason;
		}



	private:

		void _RunEncapsulated()
		{
			ke::Clock test_benchmark(m_time_unit);
			test_benchmark.start();

			Run();

			m_summary.total_time = test_benchmark.stop();
			
			if (m_aborted)
			{
				m_summary.result = TestResult::None;
				return;
			}

			for (auto& test : m_data)
			{
				(test.result == TestResult::Success) ? m_summary.success_count++ : m_summary.failure_count++;
			}

			m_summary.result = (m_summary.failure_count == 0) ? TestResult::Success : TestResult::Failure;
		}




		friend class TestManager;
	};
}


#define ASSERT_EQUAL(left, right) \
try \
{ \
	__ASSERT_EQUAL(left, right, #left, #right, __FILE__, __LINE__); \
} \
catch (...) \
{ \
	__ON_INTERRUPTION("ASSERT EQUAL", #left, #right, __FILE__, __LINE__); \
}


#define ASSERT_NOT_EQUAL(left, right) \
try \
{ \
	__ASSERT_NOT_EUQAL(left, right, #left, #right, __FILE__, __LINE__); \
} \
catch (...) \
{ \
	__ON_INTERRUPTION("ASSERT NOT EQUAL", #left, #right, __FILE__, __LINE__); \
}


#define ASSERT_SIMILAR(left, right, epsilon) \
try \
{ \
	__ASSERT_SIMILAR(left, right, epsilon, #left, #right, __FILE__, __LINE__); \
} \
catch (...) \
{ \
	__ON_INTERRUPTION("ASSERT SIMILAR", #left, #right, __FILE__, __LINE__); \
}


#define ASSERT_SIMILAR_DOUBLE(left, right, epsilon) \
try \
{ \
	__ASSERT_SIMILAR_DOUBLE(left, right, epsilon, #left, #right, __FILE__, __LINE__); \
} \
catch (...) \
{ \
	__ON_INTERRUPTION("ASSERT SIMILAR", #left, #right, __FILE__, __LINE__); \
}


#define ASSERT_NOT_SIMILAR(left, right, epsilon) \
try \
{ \
	__ASSERT_NOT_SIMILAR(left, right, epsilon, #left, #right, __FILE__, __LINE__); \
} \
catch (...) \
{ \
	__ON_INTERRUPTION("ASSERT NOT SIMILAR", #left, #right, __FILE__, __LINE__); \
}


#define ASSERT_TRUE(expr) \
try \
{ \
	__ASSERT_TRUE(expr, #expr, __FILE__, __LINE__); \
} \
catch (...) \
{ \
	__ON_INTERRUPTION("ASSERT TRUE", #expr, "true", __FILE__, __LINE__); \
}


#define ASSERT_FALSE(expr) \
try \
{ \
	__ASSERT_FALSE(expr, #expr, __FILE__, __LINE__); \
} \
catch (...) \
{ \
	__ON_INTERRUPTION("ASSERT FALSE", #expr, "false", __FILE__, __LINE__); \
}


#define ASSERT_ERROR(expr, error) \
try \
{ \
	__ASSERT_ERROR(expr, error, #expr, __FILE__, __LINE__); \
} \
catch (...) \
{ \
	__ON_INTERRUPTION("ASSERT ERROR", #expr, #error, __FILE__, __LINE__); \
}


#define ASSERT_NO_ERROR(expr) \
try \
{ \
	__ASSERT_NO_ERROR(expr, #expr, __FILE__, __LINE__); \
} \
catch (...) \
{ \
	__ON_INTERRUPTION("ASSERT NO ERROR", #expr, "no error", __FILE__, __LINE__); \
}


#define ABORT_TESTS() __ABORT_TESTS()
