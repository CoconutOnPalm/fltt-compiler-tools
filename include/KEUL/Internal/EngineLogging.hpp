#pragma once

#include "../Logging/SimpleLogger.hpp"

namespace ke
{
	namespace _internal
	{
		struct EngineLog
		{
			template <typename... Args>
			inline static void Debug(const std::format_string<Args...> format_str, Args&&... args)
			{
				ke::SimpleLogger::_EngineLog(ke::LogLayer::Debug, format_str, std::forward<Args>(args)...);
			}

			template <typename... Args>
			inline static void Info(const std::format_string<Args...> format_str, Args&&... args)
			{
				ke::SimpleLogger::_EngineLog(ke::LogLayer::Info, format_str, std::forward<Args>(args)...);
			}

			template <typename... Args>
			inline static void Warning(const std::format_string<Args...> format_str, Args&&... args)
			{
				ke::SimpleLogger::_EngineLog(ke::LogLayer::Warning, format_str, std::forward<Args>(args)...);
			}

			template <typename... Args>
			inline static void Error(const std::format_string<Args...> format_str, Args&&... args)
			{
				ke::SimpleLogger::_EngineLog(ke::LogLayer::Error, format_str, std::forward<Args>(args)...);
			}

			template <typename... Args>
			inline static void Critical(const std::format_string<Args...> format_str, Args&&... args)
			{
				ke::SimpleLogger::_EngineLog(ke::LogLayer::Critical, format_str, std::forward<Args>(args)...);
			}
		};
	}
}

