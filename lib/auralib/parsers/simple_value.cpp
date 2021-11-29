/*
 * simple_value.cpp
 */

#include "simple_value.hpp"

namespace aura
{

namespace parser
{

// TODO: make common part with simple_token
parser_if::parsed_value simple_value::parse(std::string_view& message) {
	const auto token = message.substr(0, get_token().size());
	const auto is_token_ok = (token == get_token());
	if (!is_token_ok) {
		return std::nullopt;
	}
	message.remove_prefix(get_token().size());
	const auto end = message.find(simple_value::EOL);
	if (end == std::string_view::npos) 	{
		return std::nullopt;
	}
	const auto value = message.substr(0, end);
	//TODO: there is only one line different from simple_token
	message.remove_suffix(simple_value::EOL.size());
	if (is_value_ok(value)) {
		return static_cast<std::string>(value);
	}
	return std::nullopt;
}

}

}
