/*
 * command_parser.hpp
 */

#ifndef COMMAND_PARSER_HPP_
#define COMMAND_PARSER_HPP_

#include <string_view>

namespace aura
{

namespace parser
{

class command_parser {
public:
	static bool parse(std::string_view& message);
};

}

}

#endif
