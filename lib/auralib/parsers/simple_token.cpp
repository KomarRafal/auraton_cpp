/*
 * simple_token.cpp
 */

#include "simple_token.hpp"

namespace aura
{

namespace parser
{

bool simple_token::is_value_ok(const std::string_view& value) const
{
	return value == "OK";
}

bool simple_token::parse(std::string_view& message) {
	const auto token = message.substr(0, get_token().size());
	const auto is_token_ok = (token == get_token());
	if (!is_token_ok) {
		return false;
	}
	message.remove_prefix(get_token().size());
	const auto end = message.find(simple_token::EOL);
	if (end == std::string_view::npos) 	{
		return false;
	}
	const auto source = message.substr(0, end);
	message.remove_prefix(end + simple_token::EOL.size());
	return is_value_ok(source);
}

}

}
