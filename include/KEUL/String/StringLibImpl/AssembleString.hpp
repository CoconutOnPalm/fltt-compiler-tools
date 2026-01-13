#pragma once

#include <string>
#include <ranges>
#include "StringLibTypeTraits.hpp"


namespace ke::_impl
{
    template <std::ranges::range ContainerType> 
    std::string assembleString_impl(const ContainerType& text_partitions, const std::string& separator, bool delete_last_separator = true)
    {
        if (text_partitions.empty())
            return "";

        // calculating size of the result string turns out to be slower than just appending (in most cases + my machine)

        std::string result;

        for (const auto& partition : text_partitions)
        {
            result += partition;
            result += separator;
        }

        if (delete_last_separator)
            result.erase(result.size() - separator.size(), separator.size());

        return result;
    }
}