/*
 * token_traverse.hpp
 */

#ifndef TOKEN_TRAVERSE_HPP_
#define TOKEN_TRAVERSE_HPP_

#include <string_view>
#include <string>
#include "parser_if.hpp"

namespace aura
{

namespace parser
{

class token_traverse : public parser_if
{
public:
	explicit token_traverse(const std::string& specific_token);
	virtual parsed_value parse(std::string_view& message);
	const std::string& get_token();

private:
	const std::string token;
};

}

}

#endif
