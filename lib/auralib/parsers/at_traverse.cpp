/*
 * at_traverse.cpp
 */

#include "at_traverse.hpp"

namespace aura
{

namespace parser
{

parser_if::parsed_value_t at_traverse::get_next_block()
{
	while (true) {
		const auto end = message.find(END_TOKEN);
		if (end == std::string_view::npos) 	{
			return std::nullopt;
		}
		auto block = message.substr(0, end + END_TOKEN.size());
		message.remove_prefix(end + END_TOKEN.size());
		const auto begin = block.rfind(BEGIN_TOKEN);
		if (begin == std::string_view::npos) {
			continue;
		}
		block.remove_prefix(begin);
		return static_cast<std::string>(block);
	}
}

}

}
