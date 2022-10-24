/*
 * token_traverse.cpp
 */

#include "token_traverse.hpp"

namespace aura
{

namespace parser
{

token_traverse::token_traverse(const std::string& specific_token) :
	token(specific_token) {

}

parser_if::parsed_value token_traverse::parse(std::string_view& message) {
	const auto first_token = message.substr(0, token.size());
	const auto is_token_ok = (token == first_token);
	if (!is_token_ok) {
		return std::nullopt;
	}
	// omit first token and find next or EOF
	auto next_token = message.find(token, token.size());

	auto token_content = message.substr(0, next_token);
	if (next_token == std::string::npos) {
		// remove everything
		message.remove_prefix(message.size());
	}
	else {
		message.remove_prefix(next_token);
	}
	return static_cast<std::string>(token_content);
}

const std::string& token_traverse::get_token() {
	return token;
}


}

}
