/*
 * test.cpp
 */

#include "commands/specific_command.hpp"
#include "parser_executor.hpp"
#include "specific_source.hpp"
#include "status.hpp"
#include "at.hpp"
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

parser_if::parsed_value_t test::parse(std::string_view& message)
{
	// TODO: const everywhere
	parser_if::parser_ptr_t radio_parser = std::make_unique<test_radio>();
	parser_if::parser_ptr_t flash_parser = std::make_unique<test_flash>();
	parser_if::parser_ptr_t backup_parser = std::make_unique<test_backup>();

	parser_if::parser_algorithm_t parse_algorithm;
	parse_algorithm.push_back(std::move(radio_parser));
	parse_algorithm.push_back(std::move(flash_parser));
	parse_algorithm.push_back(std::move(backup_parser));

	const auto is_test_ok = parser::parser_executor::execute(
			message,
			parse_algorithm);
	return is_test_ok;
}

const std::string test::COMMAND_TEST_TOKEN = "TEST?";

const std::string test::get_command_token()
{
	return COMMAND_TEST_TOKEN;
}

parser_if::parser_algorithm_t test_builder::build()
{
	parser_if::parser_ptr_t at_parser = std::make_unique<at>();
	parser_if::parser_ptr_t source_commnand_parser = std::make_unique<specific_source>(command::SOURCE_TOKEN);
	parser_if::parser_ptr_t test_command_parser = std::make_unique<specific_command>(test::get_command_token());
	parser_if::parser_ptr_t status_parser = std::make_unique<status>();
	parser_if::parser_ptr_t test_parser = std::make_unique<test>();

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
