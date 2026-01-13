#pragma once

#include <print>
#include <map>
#include <memory>
#include <format>
#include <mutex>


#include "LoggerPolicies.hpp"


namespace ke
{
	namespace _internal
	{
		struct EngineLog;
	}

	class SimpleLogger
	{
	private:

		LogLayer m_layer = LogLayer::Debug;
		std::unique_ptr<policies::LoggingPolicyBase> m_loggingPolicy = std::make_unique<policies::DefaultLoggingPolicy>();

		inline static std::mutex m_io_mutex;


	public:

		/**
		 * @brief Logs a message to the stderr (std::clog).
		 *
		 * @param layer			LogLayer -> will only log if the layer is above or at the current layer
		 * @param filename		name of the file that called the log function
		 * @param line			number of the line that called the log function
		 * @param format_str	std::format string
		 * @param ...args		std::format args
		 */
		template <typename... Args>
		static void logDetailed(LogLayer layer, std::string_view filename, std::string_view line, const std::format_string<Args...> format_str, Args&&... args)
		{
			std::lock_guard lock(m_io_mutex);

			if (layer < getInstance().m_layer) return; // if the layer is below the current layer, do not log

			std::string location = (!(filename.empty() || line.empty()) ? std::string(filename) + ":" + std::string(line) : std::string(filename) + std::string(line)); // if filename and line are empty, only one of them is present
			auto& logging_policy = getInstance().m_loggingPolicy;

			std::println(std::clog, "{0}{1}{2}{3}",
				logging_policy->prefix(layer, location),
				logging_policy->header(layer, location),
				ke::format(format_str, std::forward<Args>(args)...),
				logging_policy->suffix(layer, location));
		}

		/**
		 * @brief Logs a message to the stderr (std::clog).
		 *
		 * @param layer			LogLayer -> will only log if the layer is above or at the current layer
		 * @param format_str	std::format string
		 * @param ...args		std::format args
		 */
		template <typename... Args>
		static void log(LogLayer layer, const std::format_string<Args...> format_str, Args&&... args)
		{
			std::lock_guard lock(m_io_mutex);

			if (layer < getInstance().m_layer) return; // if the layer is below the current layer, do not log

			std::string location; // leave empty
			auto& logging_policy = getInstance().m_loggingPolicy;

			std::println(std::clog, "{0}{1}{2}{3}",
				logging_policy->prefix(layer, location),
				logging_policy->header(layer, location),
				ke::format(format_str, std::forward<Args>(args)...),
				logging_policy->suffix(layer, location));
			/*std::clog <<
				logging_policy->prefix(layer, location) <<
				logging_policy->header(layer, location) << ke::format(format_str, std::forward<Args>(args)...) <<
				logging_policy->suffix(layer, location) << '\n';*/
		}


		/**
		 * @brief Logs an info message to the std::clog.
		 *
		 * @param format_str	std::format string
		 * @param ...args		std::format args
		 */
		template <typename... Args>
		inline static void logInfo(const std::format_string<Args...> format_str, Args&&... args)
		{
			log(LogLayer::Info, format_str, std::forward<Args>(args)...);
		}

		/**
		 * @brief Logs a debug message to the std::clog.
		 *
		 * @param format_str	std::format string
		 * @param ...args		std::format args
		 */
		template <typename... Args>
		inline static void logDebug(const std::format_string<Args...> format_str, Args&&... args)
		{
			log(LogLayer::Debug, format_str, std::forward<Args>(args)...);
		}

		/**
		 * @brief Logs a warning message to the std::clog.
		 *
		 * @param format_str	std::format string
		 * @param ...args		std::format args
		 */
		template <typename... Args>
		inline static void logWarning(const std::format_string<Args...> format_str, Args&&... args)
		{
			log(LogLayer::Warning, format_str, std::forward<Args>(args)...);
		}

		/**
		 * @brief Logs an error message to the std::clog.
		 *
		 * @param format_str	std::format string
		 * @param ...args		std::format args
		 */
		template <typename... Args>
		inline static void logError(const std::format_string<Args...> format_str, Args&&... args)
		{
			log(LogLayer::Error, format_str, std::forward<Args>(args)...);
		}

		/**
		 * @brief Logs a critical message to the std::clog.
		 *
		 * @param format_str	std::format string
		 * @param ...args		std::format args
		 */
		template <typename... Args>
		inline static void logCritical(const std::format_string<Args...> format_str, Args&&... args)
		{
			log(LogLayer::Critical, format_str, std::forward<Args>(args)...);
		}

		/**
		 * @brief Sets the layer of the logger.
		 *
		 * @param layer	LogLayer
		 */
		static void setLayer(LogLayer layer)
		{
			std::lock_guard lock(m_io_mutex);
			getInstance().m_layer = layer;
		}


		/**
		 * @brief Sets the logging policy of the logger.
		 *
		 * @tparam LoggingPolicy	policies::LoggingPolicyBase
		 */
		template <class LoggingPolicy>
		static void setLoggingPolicy()
		{
			std::lock_guard lock(m_io_mutex);
			getInstance().m_loggingPolicy = std::make_unique<LoggingPolicy>();
		}


	private:


		template <typename... Args>
		static void _EngineLog(LogLayer layer, const std::format_string<Args...> format_str, Args&&... args)
		{
			std::lock_guard lock(m_io_mutex);

			if (layer < getInstance().m_layer) return; // if the layer is below the current layer, do not log

			std::string layerColorFmt;
			layerColorFmt = _internal::_layerToColorFmt(layer);

			std::println(std::clog, "{}{}", ke::format("[bold;green][[KENGINE]] {}[[{}]]: ", layerColorFmt, layer), std::format(format_str, std::forward<Args>(args)...));
		}



		// singleton functionality:

		SimpleLogger()
		{
		}

		~SimpleLogger() { m_destroyed = true; }

		SimpleLogger(const SimpleLogger&) = delete;
		SimpleLogger& operator=(const SimpleLogger&) = delete;

		static SimpleLogger& getInstance()
		{
			static SimpleLogger instance;

			if (instance.m_destroyed)
			{
				onDeadReference();
				new (&instance) SimpleLogger;
			}

			return instance;
		}

		static void onDeadReference()
		{
			std::println("[CRITICAL ENGINE ERROR]: dead reference in Logger - Logger is destroyed");
		}

		bool m_destroyed = false; // dead reference check

		friend struct _internal::EngineLog;
	};


#define KE_LOGINFO(format_str, ...) ke::SimpleLogger::logDetailed(ke::LogLayer::Info, __FILE__, ke::toString(__LINE__), format_str, ## __VA_ARGS__);
#define KE_LOGDEBUG(format_str, ...) ke::SimpleLogger::logDetailed(ke::LogLayer::Debug, __FILE__, ke::toString(__LINE__), format_str, ## __VA_ARGS__);
#define KE_LOGWARNING(format_str, ...) ke::SimpleLogger::logDetailed(ke::LogLayer::Warning, __FILE__, ke::toString(__LINE__), format_str, ## __VA_ARGS__);
#define KE_LOGERROR(format_str, ...) ke::SimpleLogger::logDetailed(ke::LogLayer::Error, __FILE__, ke::toString(__LINE__), format_str, ## __VA_ARGS__);
#define KE_LOGCRITICAL(format_str, ...) ke::SimpleLogger::logDetailed(ke::LogLayer::Critical, __FILE__, ke::toString(__LINE__), format_str, ## __VA_ARGS__);


} // namespace ke

