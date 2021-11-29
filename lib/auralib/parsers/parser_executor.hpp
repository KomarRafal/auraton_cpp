/*
 * parser_executor.hpp
 */

#ifndef PARSER_EXECUTOR_HPP_
#define PARSER_EXECUTOR_HPP_

#include <string_view>
#include "parser_if.hpp"

namespace aura
{

namespace parser
{

class parser_executor  {
public:
	static parser_if::parsed_value execute(std::string_view& message, const parser_if::parser_algorithm_t& parser_algorithm);
};

}

}

#endif
