#pragma once

#include <expected>
#include <vector>
#include <string>

#include "../../Error/ErrorDef.hpp"
#include "../StringLib.hpp"


namespace ke::_impl
{
    inline auto rgb_ansi_color(std::string_view arg) -> std::expected<std::string, Error>
    {
        auto args = ke::splitString<std::vector>(arg, { ",", " " });

        if (args.size() != 3)
            return std::unexpected(ke::Error::InvalidArgumentCount);

        std::string result = "\033[38;2;";

        auto r = ke::fromString<uint16_t>(args[0]);
		auto g = ke::fromString<uint16_t>(args[1]);
		auto b = ke::fromString<uint16_t>(args[2]);

		if (!(r && g && b))
			return std::unexpected(ke::Error::FormatError);

    	if (*r > 255 || *g > 255 || *b > 255)
			return std::unexpected(ke::Error::DomainError);

		return std::format("38;2;{};{};{}", r.value(), g.value(), b.value());
    }


    inline auto custom_ansi_code(std::string_view arg) -> std::expected<std::string, Error>
    {
        std::string corrected(arg);

        // support 'm' for now
        if (corrected.back() == 'm')
            corrected.pop_back();

        return corrected;
    }
}