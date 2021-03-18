/*
 * command_parser.cpp
 */

#include "command_parser.hpp"
#include "source.hpp"
#include "status.hpp"
#include "at.hpp"

namespace aura
{

namespace parser
{

bool command_parser::parse(std::string_view& message)
{
	/* TODO: const */ parser::at at_parser;
	const auto at_parser_result = at_parser.parse(message);
	if (!at_parser_result.has_value())
	{
		return false;
	}
	/* const */parser::source source_parser;
	const auto source_parser_result = source_parser.parse(message);
	if (!source_parser_result.has_value() || source_parser_result.value() != source_type::COMMAND)
	{
		return false;
	}
	/*const */parser::status status_parser;
	const auto status_parser_result = status_parser.parse(message);
	if (!status_parser_result.has_value() || status_parser_result.value() != status_type::OK)
	{
		return false;
	}
	return true;
}

}

}
