/*
 * cmd_test.cpp
 */

#include <vector>
#include "cmd_test.hpp"
#include "at.hpp"
#include "status.hpp"
#include "source_command.hpp"

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
	/*const*/ test_radio radio_parser;
	/*const*/ test_flash flash_parser;
	/*const*/ test_backup backup_parser;
	const std::vector<parser_if*> cmd_test_parser = {
			&radio_parser,
			&flash_parser,
			&backup_parser
	};
	for (auto parser : cmd_test_parser) {
		if (parser->parse(message) == false) {
			return false;
		}
	}
	return true;
}

std::vector<std::unique_ptr<parser_if>> test::parser_builder()
{
	std::unique_ptr<parser_if> at_parser = std::make_unique<at>();
	std::unique_ptr<parser_if> source_commnand_parser = std::make_unique<source_command>();
	std::unique_ptr<parser_if> test_command_parser = std::make_unique<test_command>();
	std::unique_ptr<parser_if> status_parser = std::make_unique<status>();
	std::unique_ptr<parser_if> test_parser = std::make_unique<test>();

	std::vector<std::unique_ptr<parser_if>> parse_algorithm;
	parse_algorithm.push_back(std::move(at_parser));
	parse_algorithm.push_back(std::move(source_commnand_parser));
	parse_algorithm.push_back(std::move(test_command_parser));
	parse_algorithm.push_back(std::move(status_parser));
	parse_algorithm.push_back(std::move(test_parser));
	return parse_algorithm;
}

bool test_command::is_value_ok(const std::string_view& value) const
{
	return (value == test_command::TEST_COMMAND);
}

}

}

}
