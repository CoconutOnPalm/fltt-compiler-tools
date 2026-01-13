#pragma once

#include <print>
#include <unordered_map>
#include <memory>
#include <format>
#include <mutex>
#include <iostream>
#include <fstream>


#include "LoggerPolicies.hpp"
#include "../Internal/EngineLogging.hpp"



namespace ke
{
	namespace _internal
	{
		struct EngineLog;
	}

	/**
	 * @class Logger
	 * @brief A non-static class that logs messages to the console and to files.
	 */
	class Logger
	{
	private:


		// data:

		std::ostream* m_logstream = &std::cout;
		std::unordered_map<std::string, std::ofstream> m_logfiles;

		LogLayer m_layer = LogLayer::Debug;
		std::unique_ptr<policies::LoggingPolicyBase> m_loggingPolicy = std::make_unique<policies::DefaultLoggingPolicy>();

		static inline std::mutex m_io_mutex;


	public:

		Logger() = default;
		Logger(const Logger&) = delete;
		Logger& operator=(const Logger&) = delete;

		~Logger()
		{
			std::lock_guard lock(m_io_mutex);

			for (auto& [filename, file] : m_logfiles)
			{
				file.close();
			}
		}


		/**
		 * @brief Logs a message to the target outputs.
		 * 
		 * @param layer			LogLayer -> will only log if the layer is above or at the current layer
		 * @param filename		name of the file that called the log function
		 * @param line			number of the line that called the log function
		 * @param format_str	std::format string
		 * @param ...args		std::format args
		 */
		template <typename... Args>
		void logDetailed(LogLayer layer, std::string_view filename, std::string_view line, const std::format_string<Args...> format_str, Args&&... args)
		{
			std::lock_guard lock(m_io_mutex);

			if (layer < m_layer) return; // if the layer is below the current layer, do not log

			std::string location = (!(filename.empty() || line.empty()) ? std::string(filename) + ":" + std::string(line) : std::string(filename) + std::string(line)); // if filename and line are empty, only one of them is present

			if (m_logstream)
			{
				std::println(*m_logstream, "{0}{1}{2}{3}",
					m_loggingPolicy->prefix(layer, location),
					m_loggingPolicy->header(layer, location),
					ke::format(format_str, std::forward<Args>(args)...),
					m_loggingPolicy->suffix(layer, location));
			}

			for (auto& [filename, file] : m_logfiles)
			{
				std::println(file, "{0}{1}{2}{3}",
					m_loggingPolicy->_no_ansi_prefix(layer, location),
					m_loggingPolicy->_no_ansi_header(layer, location),
					ke::format(format_str, std::forward<Args>(args)...),
					m_loggingPolicy->_no_ansi_suffix(layer, location));
			}
		}


		/**
		 * @brief Logs a message to the target outputs.
		 * 
		 * @param layer
		 * @param format_str
		 * @param ...args
		 */
		template <typename... Args>
		void log(LogLayer layer, const std::format_string<Args...> format_str, Args&&... args)
		{
			std::lock_guard lock(m_io_mutex);

			if (layer < m_layer) return; // if the layer is below the current layer, do not log

			std::string location; // leave empty

			if (m_logstream)
			{
				std::println(*m_logstream, "{0}{1}{2}{3}",
					m_loggingPolicy->prefix(layer, location),
					m_loggingPolicy->header(layer, location),
					ke::format(format_str, std::forward<Args>(args)...),
					m_loggingPolicy->suffix(layer, location));
			}

			for (auto& [filename, file] : m_logfiles)
			{
				std::println(file, "{0}{1}{2}{3}",
					m_loggingPolicy->_no_ansi_prefix(layer, location),
					m_loggingPolicy->_no_ansi_header(layer, location),
					ke::format<FormatAllowAnsiCodes::Off>(format_str, std::forward<Args>(args)...),
					m_loggingPolicy->_no_ansi_suffix(layer, location));
			}
		}


		template <typename... Args>
		inline void logInfo(const std::format_string<Args...> format_str, Args&&... args)
		{
			log(LogLayer::Info, format_str, std::forward<Args>(args)...);
		}

		template <typename... Args>
		inline void logDebug(const std::format_string<Args...> format_str, Args&&... args)
		{
			log(LogLayer::Debug, format_str, std::forward<Args>(args)...);
		}

		template <typename... Args>
		inline void logWarning(const std::format_string<Args...> format_str, Args&&... args)
		{
			log(LogLayer::Warning, format_str, std::forward<Args>(args)...);
		}

		template <typename... Args>
		inline void logError(const std::format_string<Args...> format_str, Args&&... args)
		{
			log(LogLayer::Error, format_str, std::forward<Args>(args)...);
		}

		template <typename... Args>
		inline void logCritical(const std::format_string<Args...> format_str, Args&&... args)
		{
			log(LogLayer::Critical, format_str, std::forward<Args>(args)...);
		}

		void setLayer(LogLayer layer)
		{
			std::lock_guard lock(m_io_mutex);
			m_layer = layer;
		}

		/**
		 * @brief Sets the log stream to any console std::ostream. Nullptr will turn off console logging.
		 * 
		 * @param stream console std::ostream
		 */
		void setLogStream(std::ostream* stream)
		{
			std::lock_guard lock(m_io_mutex);
			m_logstream = stream;
		}


		/**
		 * @brief Sets the logging policy of the logger.
		 *
		 * @tparam LoggingPolicy	policies::LoggingPolicyBase
		 */
		template <class LoggingPolicy>
		void setLoggingPolicy()
		{
			std::lock_guard lock(m_io_mutex);
			m_loggingPolicy = std::make_unique<LoggingPolicy>();
		}

		/**
		 * @brief Adds a log file to the logger.
		 * 
		 * @param filename
		 * @param mode
		 */
		void addLogFile(const std::string& filename, std::ios::openmode mode = std::ios::trunc)
		{
			std::lock_guard lock(m_io_mutex);

			auto empl_resoult = m_logfiles.emplace(filename, std::ofstream(filename, mode));

			if (!empl_resoult.second)
			{
				ke::_internal::EngineLog::Warning("log file \"{}\" already exists", filename);
				return;
			}

			auto& file = empl_resoult.first->second;

			if (!file.good())
			{
				ke::_internal::EngineLog::Error("Failed to open log file: {}", filename);
				file.close();
				m_logfiles.erase(empl_resoult.first);
				return;
			}
		}

		void removeLogFile(const std::string& filename)
		{
			std::lock_guard lock(m_io_mutex);

			auto it = m_logfiles.find(filename);

			if (it == m_logfiles.end())
			{
				ke::_internal::EngineLog::Warning("Log file \"{} \" does not exist", filename);
				return;
			}

			it->second.close();
			m_logfiles.erase(it);
		}

	};


} // namespace ke