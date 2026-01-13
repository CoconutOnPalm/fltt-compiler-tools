#pragma once

#include <functional>
#include <type_traits>
#include <thread>

#include "../Misc/Assert.hpp"


namespace ke
{
	class Timer
	{
		// TODO: add thread joining on destruction

	public:

		Timer() = default;

		/**
		 * @brief Calls the callback after the _Rel_time amount of time. Runs in a detached thread
		 */
		template <typename _Rep, typename _Period, class invocable_t, typename... Args>
		void start(const std::chrono::duration<_Rep, _Period>& _Rel_time, invocable_t&& callback, Args&&... args)
		{
			static_assert(std::is_invocable_v<invocable_t, Args...>, "invocable_t must be an invocable function");
			auto _callback = std::bind(std::forward<invocable_t>(callback), std::forward<Args>(args)...);

			std::thread([_Rel_time, _callback]() {
				std::this_thread::sleep_for(_Rel_time);
				_callback();
			}).detach();
		}

		/**
		 * @brief Calls the callback n times, each call separated by _Rel_time amount of time. Runs in a detached thread
		 */
		template <typename _Rep, typename _Period, class invocable_t, typename... Args>
		void repeat(const std::chrono::duration<_Rep, _Period>& _Rel_time, size_t n, invocable_t&& callback, Args&&... args)
		{
			static_assert(std::is_invocable_v<invocable_t, Args...>, "invocable_t must be an invocable function");
			auto _callback = std::bind(std::forward<invocable_t>(callback), std::forward<Args>(args)...);

			std::thread([_Rel_time, _callback, n]() {
				for (size_t i = 0; i < n; i++)
				{
					std::this_thread::sleep_for(_Rel_time);
					_callback();
				}
			}).detach();
		}

		/**
		 * @brief Calls the callback while the condition is met. Runs in a detached thread
		 */
		template <typename _Rep, typename _Period, class invocable_t, typename... Args>
		void repeatWhile(const std::chrono::duration<_Rep, _Period>& _Rel_time, const bool* const condition, invocable_t&& callback, Args&&... args)
		{
			KE_ASSERT(condition != nullptr);

			static_assert(std::is_invocable_v<invocable_t, Args...>, "invocable_t must be an invocable function");
			auto _callback = std::bind(std::forward<invocable_t>(callback), std::forward<Args>(args)...);

			std::thread([_Rel_time, _callback, condition]() mutable {

				std::this_thread::sleep_for(_Rel_time);
				while (*condition) // read only - no need for mutex
				{
					_callback();
					std::this_thread::sleep_for(_Rel_time);
				}
			}).detach();
		}


	private:


	};

} // namespace ke
