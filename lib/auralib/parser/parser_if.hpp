/*
 * parser_if.hpp
 */

#ifndef PARSER_IF_HPP_
#define PARSER_IF_HPP_

#include <string_view>

namespace aura
{

namespace parser
{

class parser_if {
	virtual bool parse(std::string_view& message) = 0;
};

}

}

#endif
