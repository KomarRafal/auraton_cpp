/*
 * parser_executor.cpp
 */

#include "parser_executor.hpp"

namespace aura
{

namespace parser
{

parser_if::parsed_value parser_executor::execute(std::string_view& message, const parser_if::parser_algorithm_t& parser_algorithm)
{
	parser_if::parsed_value status = {};
	for (auto &parser : parser_algorithm) {
		status = parser->parse(message);
		if (!status) {
			break;
		}
	}
	return status;
}

}

}
