#pragma once


#include <vector>
#include <memory>
#include <print>
#include <functional>

#include "TestBase.hpp"
#include "../Logging/Print.hpp"
#include "../Misc/Assert.hpp"



namespace ke::test::_internal
{
	class TestManager
	{
	private:

		std::vector<std::unique_ptr<TestBase>> m_tests;
		TestSummary m_summary;


	public:

		static void RunAllTests()
		{
			auto& instance = GetInstance();

			for (auto& test : instance.m_tests)
			{
				test->_RunEncapsulated();
				auto& result = test->m_summary;
				auto& summary = instance.m_summary;

				(result.result == TestResult::Success) ? summary.success_count++ : summary.failure_count++;
				summary.total_time += result.total_time;


				ke::println("[bc]TESTING: [b;bc]{}", test->m_name);

				if (result.failure_count > 0)
				{
					ke::println("[f]+-------------------------------");

					for (const auto& itr : test->m_data)
					{
						if (itr.result == TestResult::Failure)
						{
							ke::println("[f]|  > [0;br][[{}]][b;r]{}:[0;r]{} [b;r]FAILED - \"{}\":", itr.index, itr.file, itr.line, itr.test_type);

							if (itr.left_value != "expression result not formattable")
								ke::println("[f]|     [f;y]expression:  [0;y]{} [0;f]-> [0;b;y]{}", itr.left_expression, itr.left_value);
							else
								ke::println("[f]|     [f;y]expression:  [0;y]{} [0;f]-> [0;f;y]{}", itr.left_expression, itr.left_value);

							if (itr.right_value != "expression result not formattable")
								ke::println("[f]|     [f;c]expected:    [0;c]{} [0;f]-> [0;b;c]{}", itr.right_expression, itr.right_value);
							else
								ke::println("[f]|     [f;c]expected:    [0;c]{} [0;f]-> [0;f;c]{}", itr.right_expression, itr.right_value);
						}
					}
				}

				ke::println("[f]+-------------------------------");

				ke::println("[f]| [0;bg]{}[0;f;g]/{} [0;g]TEST CASES SUCCEEDED", result.success_count, result.success_count + result.failure_count);
				if (result.failure_count > 0)
					ke::println("[f]| [0;b;r]{}[0;f;r]/{} [0;r]TEST CASES FAILED", result.failure_count, result.success_count + result.failure_count);
				ke::println("[f]| [0]Test took: [0;b]{} [0]ms", result.total_time);
				ke::println("[f]+-------------------------------");
			}

			auto& summary = instance.m_summary;


			ke::println("\n[bl]SUMMARY");
			ke::println("[f;l]+-----------------------------------");
			ke::print("[f;l]| ");
			
			if (summary.failure_count == 0)
			{
				ke::println("[bg]ALL TESTS PASSED");
			}
			else
			{
				ke::println("[br]{} {} FAILED", summary.failure_count, (summary.failure_count == 1) ? "TEST" : "TESTS");

				for (auto& test : instance.m_tests)
					if (test->m_summary.failure_count > 0)
						ke::println("[f;l]| [0;f;r] > [0;br]\"{}\"[0;r] FAILED", test->m_name);
				
			}

			ke::println("[f;l]| [0]Total tests: [b]{}", summary.success_count + summary.failure_count);
			ke::println("[f;l]| [0]Total test time: [b]{} [0]ms", summary.total_time);
			ke::println("[f;l]+-----------------------------------\n");
		}


		template<typename T> requires(std::is_base_of_v<TestBase, T>)
			constexpr static void AddTest()
		{
			auto& instance = GetInstance();
			instance.m_tests.push_back(std::make_unique<T>());
		}

	private:


		TestManager() = default;

		static TestManager& GetInstance()
		{
			static TestManager instance;

			if (instance.m_destroyed)
			{
				OnDeadReference();
				new (&instance) TestManager;
			}

			return instance;
		}

		static void OnDeadReference()
		{
			std::println("[CRITICAL ENGINE ERROR]: dead reference in TestManager - Singleton is destroyed");
		}

		bool m_destroyed = false; // dead reference check


		friend class TestBase;
	};

}



#define KE_TEST(name) \
\
class _ke_test_##name : public ke::test::_internal::TestBase \
{ \
public: \
\
	_ke_test_##name() : ke::test::_internal::TestBase(#name) \
	{ \
		\
	} \
	\
	void Run() override; \
	friend class TestManager;\
}; \
\
\
struct _ke_test_##name##_register_func_ptr_t \
{ \
	_ke_test_##name##_register_func_ptr_t() \
	{ \
		ke::test::_internal::TestManager::AddTest<_ke_test_##name>(); \
	} \
std::function<void()> callback; \
}; \
\
_ke_test_##name##_register_func_ptr_t _ke_test_##name##_register_func_ptr; \
\
\
void _ke_test_##name::Run()

