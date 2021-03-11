/*
 * at.cpp
 */

#include "at.hpp"

namespace aura
{

namespace parser
{

bool at::parse(std::string_view& message) {
	const auto end = message.find(END_TOKEN);
	if (end == std::string_view::npos)
	{
		return false;
	}
	message.remove_suffix(message.size() - end);
	const auto begin = message.rfind(BEGIN_TOKEN);
	if (begin == std::string_view::npos)
	{
		return false;
	}
	message.remove_prefix(begin + BEGIN_TOKEN.size());
	return true;
}

}

}
