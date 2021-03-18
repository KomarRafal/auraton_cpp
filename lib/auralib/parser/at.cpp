/*
 * at.cpp
 */

#include "at.hpp"

namespace aura
{

namespace parser
{

std::optional<at_type> at::parse(std::string_view& message) {
	const auto end = message.find(END_TOKEN);
	if (end == std::string_view::npos) 	{
		return std::nullopt;
	}
	message.remove_suffix(message.size() - end);
	const auto begin = message.rfind(BEGIN_TOKEN);
	if (begin == std::string_view::npos) {
		return std::nullopt;
	}
	message.remove_prefix(begin + BEGIN_TOKEN.size());
	return at_type::AT_OK;
}

}

}
