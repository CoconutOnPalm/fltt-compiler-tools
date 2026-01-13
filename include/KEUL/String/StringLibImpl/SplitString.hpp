#pragma once

#include <unordered_set>

#include "StringLibTypeTraits.hpp"

namespace ke::_impl
{
    /**
     * @brief Splits a string into a container based on specified delimiters.
     *
     * @details
     * Example:
     *	text:		"apple,banana,grape;orange | strawberry,"
     *	delimiters: ",", ";"
     *	output is {"apple", "banana", "grape", "orange | strawberry"} // '|' is not a delimiter, so orange and strawberry have not been split
     * Notice that ' ' (space) is not considered a delimiter.
     *
     *	for single delimiter use ke::splitString(std::string_view, char) function
     *
     * @tparam ContainerType	Container type. Must be a dynamic range of std::string (std::vector, std::list, etc.)
     *
     * @param text			text to be split
     * @param delimiters_   a set of std::string delimiters. Can be any length	
     * 
     * @return ContainerType<std::string> of separated text. Does not include empty strings
     */
    template <_StringRangeTypeConcept ContainerType>
    inline auto splitString_impl(std::string_view text, const std::unordered_set<std::string>& delimiters)
    {
        if (text.empty() || delimiters.empty())
            return ContainerType({std::string(text)});

        ContainerType output{};

        size_t start = 0; 
        size_t i = 0;
        for (; i < text.size(); i++)
        {

            for (const auto& delim : delimiters)
            {
                if (text.size() - i < delim.size())
                    continue;

                if (text.substr(i, delim.size()) == delim)
                {
                    if (start != i)
                        output.emplace_back(text.begin() + start, text.begin() + i);

                    start = i + delim.size();
                }
            }
        }

        if (start != i)
        {
            output.emplace_back(text.begin() + start, text.begin() + i);
        }

        return output;
    }

    /**
     * @brief Splits a string into a container based on specified delimiters.
     *
     * @details
     * Example:
     *	text:		"apple,banana,grape;orange | strawberry,"
     *	delimiters: ",", ";"
     *	output is {"apple", "banana", "grape", "orange | strawberry"} // '|' is not a delimiter, so orange and strawberry have not been split
     * Notice that ' ' (space) is not considered a delimiter.
     *
     *	for single delimiter use ke::splitString(std::string_view, char) function
     *
     * @tparam ContainerType	Container type. Must be a dynamic container of std::string (std::set, std::queue, etc.)
     *
     * @param text			text to be split
     * @param delimiters	a set of std::string delimiters. Can be any length
     *
     * @return ContainerType<std::string> of separated text. Does not include empty strings
     */
    template <_StringContainerTypeConcept ContainerType>
    inline auto splitString_impl(std::string_view text, const std::unordered_set<std::string>& delimiters)
    {
        if (text.empty() || delimiters.empty())
            return ContainerType({std::string(text)});

        ContainerType output{};

        size_t start = 0;
        size_t i = 0;
        for (; i < text.size(); i++)
        {
            for (const auto& delim : delimiters)
            {
                if (text.size() - i < delim.size())
                    continue;

                if (text.substr(i, delim.size()) == delim)
                {
                    if (start != i)
                        output.emplace(text.begin() + start, text.begin() + i);

                    start = i + delim.size();
                }
            }
        }

        if (start != i)
        {
            output.emplace(text.begin() + start, text.begin() + i);
        }

        return output;
    }

    /**
     * Splits a string into a container based on specified delimiters.
     *
     * @details
     * Example:
     *	text:		"apple,banana,grape; onion'
     *	delimiter: ','
     *	output is {"apple", "banana", "grape; onion"} // Neither ';' nor ' ' (space) are delimiters, so "grape; onion" has not been split
     *
     *	for multiple delimiters use ke::splitString(std::string_view, std::string_view) function
     *
     * @tparam ContainerType	Container type. Must be a dynamic range of std::string (std::vector, std::list, etc.)
     *
     * @param text			text to be split
     * @param delimiter		delimiter - must be single character
     *
     * @return ContainerType<std::string> of separated text. Does not include empty strings
     */
    template <_StringRangeTypeConcept ContainerType>
    inline auto splitString_impl(std::string_view text, const char delimiter)
    {
        if (text.empty())
            return ContainerType({std::string(text)});

        ContainerType output{};

        size_t start = 0;
        size_t i = 0;
        for (; i < text.size(); i++)
        {
            // index i hit delimiter
            if (text[i] == delimiter)
            {
                if (start != i)
                {
                    output.emplace_back(text.begin() + start, text.begin() + i);
                }

                start = i + 1;
            }
        }

        if (start != i)
        {
            output.emplace_back(text.begin() + start, text.begin() + i);
        }

        return output;
    }

    /**
     * @brief Generalization of other splitTo... functions. Splits a string into a container based on specified delimiters.
     *
     * @details
     * Example:
     *	text:		"apple,banana,grape; onion'
     *	delimiter: ','
     *	output is {"apple", "banana", "grape; onion"} // Neither ';' nor ' ' (space) are delimiters, so "grape; onion" has not been split
     *
     *	for multiple delimiters use ke::splitString(std::string_view, std::string_view) function
     *
     * @tparam ContainerType	Container type. Must be a dynamic container of std::string (std::set, std::queue, etc.)
     *
     * @param text			text to be split
     * @param delimiter		delimiter - must be single character
     *
     * @return ContainerType<std::string> of separated text. Does not include empty strings
     */
    template <_StringContainerTypeConcept ContainerType>
    inline auto splitString_impl(std::string_view text, const char delimiter)
    {
        if (text.empty())
            return ContainerType({std::string(text)});

        ContainerType output{};

        size_t start = 0;
        size_t i = 0;
        for (; i < text.size(); i++)
        {
            // index i hit delimiter
            if (text[i] == delimiter)
            {
                if (start != i)
                {
                    output.emplace(text.begin() + start, text.begin() + i);
                }

                start = i + 1;
            }
        }

        if (start != i)
        {
            output.emplace(text.begin() + start, text.begin() + i);
        }

        return output;
    }

    /**
     * @brief Splits a string into a pair based on specified delimiters.
     * 
     * @tparam ContainerType 
     * @param text          text to be split
     * @param delimiter     delimiter - must be single character
     * @param split_index   the split will be performed on n-th delimiter occurence. Default is 0. Does nothing if there are less than split_index delimiters.
     * @return auto 
     */
    template <_StringPairTypeConcept ContainerType>
    inline auto splitStringToPair_impl(std::string_view text, const char delimiter, size_t split_index = 0)
    {
        if (text.empty())
            return ContainerType("", "");

        uint64_t occurences = 0;

        for (size_t i = 0; i < text.size(); i++)
        {
            if (text[i] == delimiter)
            {
                occurences++;

                if (occurences > split_index)
                    return ContainerType(std::string(text.begin(), text.begin() + i), std::string(text.begin() + i + 1, text.end()));
            }
        }

        return ContainerType(std::string(text), "");
    }


    template <_StringPairTypeConcept ContainerType, typename... Args>
    inline auto splitStringToPair_impl(std::string_view text, const std::unordered_set<std::string>& delimiters, size_t split_index = 0)
    {
        if (text.empty())
            return ContainerType("", "");

        uint64_t occurences = 0;

        for (size_t i = 0; i < text.size(); i++)
        {
            for (const auto& delim : delimiters)
            {
                if (i + delim.size() < text.size() && text.substr(i, delim.size()) == delim)
                {
                    occurences++;

                    if (occurences > split_index)
                        return ContainerType(std::string(text.begin(), text.begin() + i), std::string(text.begin() + i + delim.size(), text.end()));
                }
            }
        }

        return ContainerType(std::string(text), "");
    }
}