/*
 * command_parser.cpp
 */
#include <vector>
#include "command_parser.hpp"
#include "specific_source.hpp"
#include "command.hpp"
#include "status.hpp"
#include "at.hpp"

namespace aura
{

namespace parser
{
//TODO: DELETE this file?
bool command_parser::parse(std::string_view& message)
{
	/* TODO: const */ parser::at at_parser;
	/* const */parser::specific_source source_command_parser(parser::command::SOURCE_TOKEN);
	/*const */parser::status status_parser;
	const std::vector<parser_if*> command_parser = {
			&at_parser,
			&source_command_parser,
			&status_parser
	};
	for (auto parser : command_parser) {
		if (parser->parse(message) == false) {
			return false;
		}
	}
	return true;
}

}

}
