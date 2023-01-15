/*
 * command_parser.hpp
 */

#ifndef COMMAND_PARSER_HPP_
#define COMMAND_PARSER_HPP_

#include <string_view>
#include "parser_if.hpp"

namespace aura
{

namespace parser
{

class command_parser : public parser_if {
public:
	static const uint16_t MAX_COMMAND_LENGTH = 100;
	static parsed_value_t parse(std::string_view& message, const std::string& command);
};

class command_parser_builder
{
public:
	static parser_if::parser_algorithm_t build(const std::string& command);
};

}

}

#endif
