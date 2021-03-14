/*
 * simple_token.hpp
 */

#ifndef SIMPLE_TOKEN_HPP_
#define SIMPLE_TOKEN_HPP_

#include <string>
#include <map>
#include "parser_if.hpp"

namespace aura
{

namespace parser
{

template <class TOKEN_TYPE>
class simple_token : public parser_if<TOKEN_TYPE> {
public:
	using token_map = std::map<std::string, TOKEN_TYPE>;
	virtual std::optional<TOKEN_TYPE> parse(std::string_view& message);
	virtual const std::string& get_token() const = 0;
	virtual const token_map& get_token_map() const = 0;
};

template <class TOKEN_TYPE>
std::optional<TOKEN_TYPE> simple_token<TOKEN_TYPE>::parse(std::string_view& message) {
	const auto token = message.substr(0, get_token().size());
	const auto is_token_ok = (token == get_token());
	if (!is_token_ok) {
		return std::nullopt;
	}
	message.remove_prefix(get_token().size());
	const auto end = message.find(simple_token<TOKEN_TYPE>::EOL);
	if (end == std::string_view::npos) 	{
		return std::nullopt;
	}
	const auto source = message.substr(0, end);
	message.remove_prefix(end + simple_token<TOKEN_TYPE>::EOL.size());
	for (auto token_pair : get_token_map()) {
		if (token_pair.first == static_cast<std::string>(source)) {
			return token_pair.second;
		}
	}
	return std::nullopt;
}

}

}

#endif
