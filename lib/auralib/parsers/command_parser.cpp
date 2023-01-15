/*
 * command_parser.cpp
 */
#include <vector>
#include "commands/specific_command.hpp"
#include "parser_executor.hpp"
#include "specific_source.hpp"
#include "command_parser.hpp"
#include "status.hpp"
#include "at.hpp"

namespace aura
{

namespace parser
{

parser_if::parser_algorithm_t command_parser_builder::build(const std::string& command)
{
	// TODO: const everywhere
	parser_if::parser_ptr_t at_parser = std::make_unique<at>();
	parser_if::parser_ptr_t source_commnand_parser = std::make_unique<specific_source>(command::SOURCE_TOKEN);
	parser_if::parser_ptr_t specific_command_parser = std::make_unique<commands::specific_command>(command);
	parser_if::parser_ptr_t status_parser = std::make_unique<status>();

	parser_if::parser_algorithm_t parse_algorithm;
	parse_algorithm.push_back(std::move(at_parser));
	parse_algorithm.push_back(std::move(source_commnand_parser));
	parse_algorithm.push_back(std::move(specific_command_parser));
	parse_algorithm.push_back(std::move(status_parser));
	return parse_algorithm;
}

parser_if::parsed_value_t command_parser::parse(std::string_view& message, const std::string& command)
{
	const auto is_command_ok = parser::parser_executor::execute(
			message,
			command_parser_builder::build(command));
	return is_command_ok;
}

}

}
