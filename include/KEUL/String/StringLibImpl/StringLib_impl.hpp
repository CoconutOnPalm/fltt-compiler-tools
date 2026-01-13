#pragma once

#include <string>
#include <set>
#include <cstdint>
#include <limits>
#include <string_view>

#include "SplitString.hpp"
#include "AssembleString.hpp"

namespace ke
{
	namespace _impl
	{
		/**
		 * @brief Converts a value to a string. If the conversion fails, returns error string.
		 *
		 * @tparam T			Type of the value to be converted to a string
		 *
		 * @param arg			Value to be converted to a string
		 * @param default_str	Default string to be returned if the conversion fails
		 *
		 * @return
		 */
		template <typename T>
		std::string tryToString_impl(const T& arg, const std::string& default_str = "???")
		{
			if constexpr (std::formattable<T, char>)
				return std::format("{}", arg);
			else
			{
				std::stringstream ss{}; ss << arg;
				T value{};

				ss >> value;

				if (ss.fail())
					return default_str;

				return ss.str();
			}
		}


		/**
		 * @brief removes specified whitespaces from the front and back of a string.
		 *
		 * @example
		 *  str = "  hello world  " aka "\t hello wordl \t\n"
		 *	trimString_impl(str, {' ', '\t', '\n'}): str -> "hello world"
		 *
		 * @param str			Input string. Lines are not trimmed.
		 * @param whitespaces
		 */
		inline void trimString_impl(std::string& str, std::initializer_list<char> whitespaces);

		/**
		 * @brief removes comments from a string. Comments are marked by "//" and end with a newline character.
		 *
		 * @example
		 *  str =
		 *	R""""(
		 *	int main()
		 *	{
		 *		// this is a comment
		 *		return 0;
		 *	}
		 *	)""""
		 * 	removeComments_impl(str): str ->
		 *	R""""(
		 *	int main()
		 *	{
		 *
		 *		return 0;
		 *	}
		 *	)""""
		 *
		 * @param str Input string. Can have multiple lines.
		 */
		inline void removeComments_impl(std::string& str);


		/**
		 * @brief Shortens a string to a specified length. If the string is shorter than the specified length, it is not modified.
		 *
		 * @example
		 *  str = "Hello, World!"
		 *	shortenString_impl(str, 10): str -> "Hello W..."
		 *
		 * @param str			Input string
		 * @param max_length	Maximum length of the string
		 */
		inline void shortenString_impl(std::string& str, size_t max_length)
		{
			if (max_length <= 3 && str.length() > max_length)
			{
				str.clear();
				str = std::string(max_length, '.');
				return;
			}

			if (str.length() > max_length)
			{
				str = str.substr(0, max_length - 3) + "...";
			}
		}


		// WARNING: works only with MSVC compiler
		/**
		 * @brief [MSVC ONLY!]	Cleans type information from a string. Removes "class ", "struct ", "enum ", "union ", "typedef " from the string.
		 *
		 * @param str
		 */
		inline void cleanTypeInfo_impl(std::string& str)
		{
			if (str.empty())
				return;

			size_t find_class = str.find("class ");
			while (find_class != str.npos)
			{
				str.replace(find_class, 6, "");
				find_class = str.find("class ");
			}

			size_t find_struct = str.find("struct ");
			while (find_struct != str.npos)
			{
				str.replace(find_struct, 7, "");
				find_struct = str.find("struct ");
			}

			size_t find_enum = str.find("enum ");
			while (find_enum != str.npos)
			{
				str.replace(find_enum, 5, "");
				find_enum = str.find("enum ");
			}

			size_t find_union = str.find("union ");
			while (find_union != str.npos)
			{
				str.replace(find_union, 6, "");
				find_union = str.find("union ");
			}

			size_t find_typedef = str.find("typedef ");
			while (find_typedef != str.npos)
			{
				str.replace(find_typedef, 8, "");
				find_typedef = str.find("typedef ");
			}
		}


		

		/**
		 * @brief removes specified whitespaces from the front and back of a string.
		 *
		 * @param str
		 * @param whitespaces
		 */
		inline void trimString_impl(std::string& str, std::initializer_list<char> whitespaces)
		{
			if (str.size() > static_cast<size_t>(std::numeric_limits<int64_t>::max()))
			{
				return;
			}

			int64_t start = 0;
			int64_t count = str.length();

			std::set<char> wsp = whitespaces;

			for (int64_t i = 0; i < static_cast<int64_t>(str.length()); ++i)
			{
				if (wsp.find(str[i]) == wsp.end()) // not a whitespace
				{
					start = i;
					break;
				}
			}

			for (int64_t i = static_cast<int64_t>(str.length() - 1); i > 0; --i)
			{
				if (wsp.find(str[i]) == wsp.end()) // not a whitespace
				{
					count = i + 1 - start;
					break;
				}
			}

			str = str.substr(start, count);
		}

		/**
		 * @brief removes comments from a string. Comments are marked by "//" and end with a newline character.
		 *
		 * @param str
		 */
		inline void removeComments_impl(std::string& str, const std::string_view comment_character = "//")
		{
			bool is_comment = false;
			size_t comment_start = 0;

			for (size_t i = 0; i < str.size() - (comment_character.size() - 1); ++i)
			{
				if (str[i] == '\n')
				{
					if (is_comment)
					{
						str.erase(comment_start, i - comment_start);
						i = comment_start;
					}
					is_comment = false;
					continue;
				}

				if (is_comment)
					continue;

				if (str.substr(i, comment_character.size()) == comment_character)
				{
					comment_start = i;
					is_comment = true;
					continue;
				}
			}

			if (is_comment)
				str.erase(comment_start, str.length() - comment_start);
		}



		// definitions: 

		//template<_StringRangeTypeConcept ContainerType>
		//inline auto splitString_impl(std::string_view text, std::string_view delimiters)
		


		//template<_StringContainerTypeConcept ContainerType>
		//inline auto splitString_impl(std::string_view text, std::string_view delimiters)
		


		//template<_StringRangeTypeConcept ContainerType>
		//inline auto splitString_impl(std::string_view text, const char delimiter)
		


		//template<_StringContainerTypeConcept ContainerType>
		//inline auto splitString_impl(std::string_view text, const char delimiter)
		


		


	} // namespace _impl

} // namespace ke
