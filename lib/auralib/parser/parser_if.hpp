/*
 * parser_if.hpp
 */

#ifndef PARSER_IF_HPP_
#define PARSER_IF_HPP_

#include <string_view>
#include <optional>

namespace aura
{

namespace parser
{

template <class TOKEN_TYPE>
class parser_if {
public:
	virtual std::optional<TOKEN_TYPE> parse(std::string_view& message) = 0;
	const std::string EOL = "\r\n";
};

}

}

#endif
