/*
 * simple_value.cpp
 */

#include "simple_value.hpp"

namespace aura
{

namespace parser
{

// TODO: make common part with simple_token
bool simple_value::parse(std::string_view& message) {
	const auto token = message.substr(0, get_token().size());
	const auto is_token_ok = (token == get_token());
	if (!is_token_ok) {
		return false;
	}
	message.remove_prefix(get_token().size());
	const auto end = message.find(simple_value::EOL);
	if (end == std::string_view::npos) 	{
		return false;
	}
	const auto value = message.substr(0, end);
	//TODO: there is only one line different from simple_token
	message.remove_suffix(simple_value::EOL.size());
	return is_value_ok(value);
}

}

}
