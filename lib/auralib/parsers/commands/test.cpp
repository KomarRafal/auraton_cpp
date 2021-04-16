/*
 * test.cpp
 */

#include "parsers/commands/specific_command.hpp"
#include "parsers/specific_source.hpp"
#include "parsers/status.hpp"
#include "parsers/at.hpp"
#include "test.hpp"

namespace aura
{

namespace parser
{

namespace commands
{

const std::string& test_radio::get_token() const
{
	return RADIO_TOKEN;
}

const std::string& test_flash::get_token() const
{
	return FLASH_TOKEN;
}

const std::string& test_backup::get_token() const
{
	return BACKUP_TOKEN;
}

bool test::parse(std::string_view& message)
{
	// TODO: consts
	/*const*/ test_radio radio_parser;
	/*const*/ test_flash flash_parser;
	/*const*/ test_backup backup_parser;
	const std::vector<parser_if*> cmd_test_parser = {
			&radio_parser,
			&flash_parser,
			&backup_parser
	};
	// TODO: parser_executor
	for (auto parser : cmd_test_parser) {
		if (parser->parse(message) == false) {
			return false;
		}
	}
	return true;
}

const std::string test::COMMAND_TEST_TOKEN = "TEST?";

const std::string test::get_command_token()
{
	return COMMAND_TEST_TOKEN;
}

parser_if::parser_algorithm_t test_builder::build()
{
	parser_if::parser_ptr at_parser = std::make_unique<at>();
	parser_if::parser_ptr source_commnand_parser = std::make_unique<specific_source>(command::SOURCE_TOKEN);
	parser_if::parser_ptr test_command_parser = std::make_unique<specific_command>(test::get_command_token());
	parser_if::parser_ptr status_parser = std::make_unique<status>();
	parser_if::parser_ptr test_parser = std::make_unique<test>();

	parser_if::parser_algorithm_t parse_algorithm;
	parse_algorithm.push_back(std::move(at_parser));
	parse_algorithm.push_back(std::move(source_commnand_parser));
	parse_algorithm.push_back(std::move(test_command_parser));
	parse_algorithm.push_back(std::move(status_parser));
	parse_algorithm.push_back(std::move(test_parser));
	return parse_algorithm;
}

}

}

}
