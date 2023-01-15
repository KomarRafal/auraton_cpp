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

parser_if::parsed_value_t simple_token::parse(std::string_view& message) {
	const auto token = message.substr(0, get_token().size());
	const auto is_token_ok = (token == get_token());
	if (!is_token_ok) {
		return std::nullopt;
	}
	message.remove_prefix(get_token().size());
	const auto end = message.find(parser_if::EOL);
	const auto value = message.substr(0, end);
	if (end == std::string::npos) {
		// remove everything
		message.remove_prefix(message.size());
	}
	else {
		message.remove_prefix(end + parser_if::EOL.size());
	}

	if (is_value_ok(value)) {
		return static_cast<std::string>(value);
	}
	return std::nullopt;
}

}

}
