#pragma once

#include <string>
#include <map>
#include <format>

namespace ke
{

	enum class Error
	{
		Error,

		// logic errors
		InvalidArgument,
		InvalidArgumentCount,
		DomainError,		// aka range error
		LengthError,
		OutOfRange,

		// runtime errors
		OverflowInt,
		OverflowFloat,
		UnderflowInt,
		UnderflowFloat,
		DivisionByZero,
		FormatError,
		SystemError,
		EmptyArgument,

		// file errors
		FileNotFound,
		FileNotOpen,
		FileNotReadable,
		FileNotWritable,
		FileCorrupted,
		InvalidFileStructure,

		// memory errors
		OutOfMemory,
		InvalidMemoryAccess,

		_Count,

		None = -1,
		UnexpectedError = -2
	};


	namespace _internal
	{
		inline const std::string& _errorToString(ke::Error error)
		{
			const static std::map<Error, std::string> errorStrings = {
				{ Error::Error,					"Error" },
				{ Error::InvalidArgument,		"Invalid Argument" },
				{ Error::InvalidArgumentCount,	"Invalid Argument Count" },
				{ Error::DomainError,			"Domain Error" },
				{ Error::LengthError,			"Length Error" },
				{ Error::OutOfRange,			"Out of Range" },
				{ Error::OverflowInt,			"Overflow Int" },
				{ Error::OverflowFloat,			"Overflow Float" },
				{ Error::UnderflowInt,			"Underflow Int" },
				{ Error::UnderflowFloat,		"Underflow Float" },
				{ Error::DivisionByZero,		"Division by Zero" },
				{ Error::FormatError,			"Format Error" },
				{ Error::SystemError,			"System Error" },
				{ Error::EmptyArgument,			"Empty Argument" },
				{ Error::FileNotFound,			"File Not Found" },
				{ Error::FileNotOpen,			"File Not Open" },
				{ Error::FileNotReadable,		"File Not Readable" },
				{ Error::FileNotWritable,		"File Not Writable" },
				{ Error::FileCorrupted,			"File Corrupted" },
				{ Error::InvalidFileStructure,	"Invalid File Structure" },
				{ Error::OutOfMemory,			"Out of Memory" },
				{ Error::InvalidMemoryAccess,	"Invalid Memory Access" },
				{ Error::None,					"None" }
			};

			const static std::string unknownError = std::format("Unknown Error type: {}", static_cast<int>(error));

			auto it = errorStrings.find(error);
			if (it != errorStrings.end())
				return it->second;
			else
				return unknownError;
		}

	} // namespace _internal

} // namespace ke


template <>
struct std::formatter<ke::Error> : std::formatter<std::string>
{
	auto format(const ke::Error& error, std::format_context& ctx) const
	{
		return std::format_to(ctx.out(), "{}", ke::_internal::_errorToString(error));
	}
};