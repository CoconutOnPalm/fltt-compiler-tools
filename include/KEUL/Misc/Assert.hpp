#pragma once


#include "../Logging/SimpleLogger.hpp"


namespace ke::_internal
{
	inline void _assert(bool result, std::string_view expression, std::string_view file, uint32_t line)
	{
		if (!result)
		{
			ke::SimpleLogger::logCritical("[b;r]ASSERTION FAILED: [0]{}\n[f]  |{}:{}", expression, file, line);
			std::abort();
		}
	}
}


#define KE_ASSERT(expression) ::ke::_internal::_assert(expression, #expression, __FILE__, __LINE__)
