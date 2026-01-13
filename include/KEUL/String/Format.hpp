#pragma once

#include <format>

#include "FormatImpl/FormatImpl.hpp"
#include "../Misc/Range.hpp"

#include "FormatImpl/CustomFormatters.hpp"

namespace ke
{
	/**
	 * @brief std::format -like formatting with a possibility to add ANSI escape sequences in [ ] braces.
	 *
	 * Basic code aliases:
	 *
	 * @li	[reset], [0] - resets all attributes
	 * @li	[bold], [b], [1] - bold text
	 * @li	[dim], [d], [2] - dim text
	 * @li	[faint], [f], [2] - faint text
	 * @li	[italic], [i], [3] - italic text
	 * @li	[underline], [u], [4] - underline text
	 * @li	[blink], [5] - blink text
	 * @li	[reverse], [rev], [7] - reverse text
	 * @li	[hidden], [hide], [8] - hidden text
	 * @li	[sstrikethrough], [strike], [9] - strikethrough text
	 *
	 * @li	[black], [k], [30] - black text
	 * @li	[red], [r], [31] - red text
	 * @li	[green], [g], [32] - green text
	 * @li	[yellow], [y], [33] - yellow text
	 * @li	[blue], [l], [34] - blue text
	 * @li	[magenta], [m], [35] - magenta text
	 * @li	[cyan], [c], [36] - cyan text
	 * @li	[white], [w], [37] - white text
	 * @li	[default], [def], [39] - default text
	 *
	 * @see Format.h (bottom) for more aliases.
	 *
	 *
	 *
	 * @tparam AllowAnsiCodes Allow/Disallow ANSI codes in the output string.
	 * @tparam Args Arguments to format.
	 * @param format_str Format string.
	 * @param args Arguments to format.
	 * @return Formatted string.
	 */
	template <FormatAllowAnsiCodes AllowAnsiCodes = FormatAllowAnsiCodes::On, typename... Args>
	inline std::string format(const std::format_string<Args...> format_str, Args&&... args)
	{
		return _impl::format_impl<AllowAnsiCodes>(format_str, std::forward<Args>(args)...);
	}

	/**
	 * @brief ke::format but removes ANSI codes from the output string.
	 * @tparam Args Arguments to format.
	 * @param format_str Format string.
	 * @param args Arguments to format.
	 * @return Formatted string.
	 */
	template <typename... Args>
	inline std::string format_no_ansi(const std::format_string<Args...> format_str, Args&&... args)
	{
		return _impl::format_impl<FormatAllowAnsiCodes::Off>(format_str, std::forward<Args>(args)...);
	}


	/* ALL ANSI CODE ALIASES
	 
	[reset], [0]:											resets the ansi formatting

	MODIFIERS
	[bold], [b], [1]:										bold text
	[dim], [d], [faint], [f], [2]:							fainted text
	[italic], [i], [3]:										italic text
	[underline], [u], [4]:									underlined text
	[blink], [5]:											blinking text
	[reverse], [rev], [7]:									reverse video
	[hidden], [hide], [8]:									hidden text
	[strikethrough], [strike], [9]:							strikethrough text

	FOREGROUND COLORS
	[black], [k], [30]:										black text
	[red], [r], [31]:										red text
	[green], [g], [32]:										green text
	[yellow], [y], [33]:									yellow text
	[blue], [l], [34]:										blue text
	[magenta], [m], [35]:									magenta text
	[cyan], [c], [36]:										cyan text
	[white], [w], [37]:										white text
	[default], [def], [39]:									default text color

	BRIGHT FOREGROUND COLORS
	[bblack], [bk], [brightblack], [90]:					bright black text
	[bred], [br], [brightred], [91]:						bright red text
	[bgreen], [bg], [brightgreen], [92]:					bright green text
	[byellow], [by], [brightyellow], [93]:					bright yellow text
	[bblue], [bb], [brightblue], [94]:						bright blue text
	[bmagenta], [bm], [brightmagenta], [95]:				bright magenta text
	[bcyan], [bc], [brightcyan], [96]:						bright cyan text
	[bwhite], [bw], [brightwhite], [97]:					bright white text

	BACKGROUND COLORS
	[bg_black], [backblack], [40]:							black background
	[bg_red], [backred], [41]:								red background
	[bg_green], [backgreen], [42]:							green background
	[bg_yellow], [backyellow], [43]:						yellow background
	[bg_blue], [backblue], [44]:							blue background
	[bg_magenta], [backmagenta], [45]:						magenta background
	[bg_cyan], [backcyan], [46]:							cyan background
	[bg_white], [backwhite], [47]:							white background
	[bg_default], [backdef], [49]:							default background color

	BRIGHT BACKGROUND COLORS
	[brightbackblack], [bbackblack], [bbackk], [100]:		bright black background
	[brightbackred], [bbackred], [bbackr], [101]:			bright red background
	[brightbackgreen], [bbackgreen], [bbackg], [102]:		bright green background
	[brightbackyellow], [bbackyellow], [bbacky], [103]:		bright yellow background
	[brightbackblue], [bbackblue], [bbackb], [104]:			bright blue background
	[brightbackmagenta], [bbackmagenta], [bbackm], [105]:	bright magenta background
	[brightbackcyan], [bbackcyan], [bbackc], [106]:			bright cyan background
	[brightbackwhite], [bbackwhite], [bbackw], [107]:		bright white background

	*/

}