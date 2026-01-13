#pragma once

#include "LoggerDef.hpp"
#include <chrono>

#include "../String/Format.hpp"



namespace ke
{
	namespace _internal
	{
		inline std::string _layerToColorFmt(const ke::LogLayer layer)
		{
			const static std::map<ke::LogLayer, std::string> s_logLayerToFmtColor = {
				{ke::LogLayer::Info, 		"[bold;cyan]"},
				{ke::LogLayer::Debug, 		"[bold;blue]"},
				{ke::LogLayer::Warning, 	"[bold;yellow]"},
				{ke::LogLayer::Error, 		"[bold;red]"},
				{ke::LogLayer::Critical, 	"[bold;magenta]"},
				{ke::LogLayer::Off, 		""},
			};

			return s_logLayerToFmtColor.at(layer);
		}
	}


	namespace policies
	{
		/**
		 * @brief Logging policy base class.
		 * @details Log message structure: [prefix] [header] <message> <additional_info> [suffix]
		 */
		struct LoggingPolicyBase
		{
			virtual ~LoggingPolicyBase() = default;

			virtual std::string prefix(LogLayer layer, std::string_view location = "") const = 0;
			virtual std::string header(LogLayer layer, std::string_view location = "") const = 0;
			virtual std::string suffix(LogLayer layer, std::string_view location = "") const = 0;

			virtual std::string _no_ansi_prefix(LogLayer layer, std::string_view location = "") const = 0;
			virtual std::string _no_ansi_header(LogLayer layer, std::string_view location = "") const = 0;
			virtual std::string _no_ansi_suffix(LogLayer layer, std::string_view location = "") const = 0;
		};


		struct DefaultLoggingPolicy : public LoggingPolicyBase
		{
			std::string prefix(LogLayer layer, std::string_view location = "") const override
			{
				const auto time = std::chrono::system_clock::now();
				return ke::format("[f]{:%Y-%m-%d %X} {}\n", time, location);
			}

			std::string header(LogLayer layer, std::string_view location = "") const override
			{
				std::string layerColorFmt = ke::_internal::_layerToColorFmt(layer);
				return ke::format("{}[[{}]]: ", layerColorFmt, layer);
			}

			std::string suffix(LogLayer layer, std::string_view location = "") const override
			{
				return "";
			}
			

			std::string _no_ansi_prefix(LogLayer layer, std::string_view location = "") const override
			{
				const auto time = std::chrono::system_clock::now();
				return ke::format<FormatAllowAnsiCodes::Off>("[f]{:%Y-%m-%d %X} {}\n", time, location);
			}

			std::string _no_ansi_header(LogLayer layer, std::string_view location = "") const override
			{
				std::string layerColorFmt = ke::_internal::_layerToColorFmt(layer);
				return ke::format<FormatAllowAnsiCodes::Off>("{}[[{}]]: ", layerColorFmt, layer);
			}

			std::string _no_ansi_suffix(LogLayer layer, std::string_view location = "") const override
			{
				return "";
			}
		};

		struct CompactLoggingPolicy : public LoggingPolicyBase
		{
			std::string prefix(LogLayer layer, std::string_view location = "") const override
			{
				return "";
			}

			std::string header(LogLayer layer, std::string_view location = "") const override
			{
				std::string layerColorFmt = ke::_internal::_layerToColorFmt(layer);
				return ke::format("{}[[{}]]: ", layerColorFmt, layer);
			}

			std::string suffix(LogLayer layer, std::string_view location = "") const override
			{
				return "";
			}


			std::string _no_ansi_prefix(LogLayer layer, std::string_view location = "") const override
			{
				return "";
			}

			std::string _no_ansi_header(LogLayer layer, std::string_view location = "") const override
			{
				std::string layerColorFmt = ke::_internal::_layerToColorFmt(layer);

				return ke::format<FormatAllowAnsiCodes::Off>("{}[[{}]]: ", layerColorFmt, layer);
			}

			std::string _no_ansi_suffix(LogLayer layer, std::string_view location = "") const override
			{
				return "";
			}
		};


		struct BareLoggingPolicy : public LoggingPolicyBase
		{
			std::string prefix(LogLayer layer, std::string_view location = "") const override { return ""; }
			std::string header(LogLayer layer, std::string_view location = "") const override { return ""; }
			std::string suffix(LogLayer layer, std::string_view location = "") const override { return ""; }


			std::string _no_ansi_prefix(LogLayer layer, std::string_view location = "") const override { return ""; }
			std::string _no_ansi_header(LogLayer layer, std::string_view location = "") const override { return ""; }
			std::string _no_ansi_suffix(LogLayer layer, std::string_view location = "") const override { return ""; }
		};
	}
}
