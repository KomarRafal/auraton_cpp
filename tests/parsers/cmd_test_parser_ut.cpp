/*
 * cmd_test_parser_ut.cpp
 */

#include "gtest/gtest.h"
#include "parsers/cmd_test.hpp"

TEST(cmd_test_parser_ut, cmd_test_test_simple_ok)
{
	const std::string test_string {
		"RADIO:OK\r\n"
		"FLASH:OK\r\n"
		"BACKUP:OK\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::commands::test parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_TRUE(parse_result);
}

TEST(cmd_test_parser_ut, cmd_test_radio_error)
{
	const std::string test_string {
		"RADIO:ERROR\r\n"
		"FLASH:OK\r\n"
		"BACKUP:OK\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::commands::test parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_FALSE(parse_result);
}

TEST(cmd_test_parser_ut, cmd_test_flash_error)
{
	const std::string test_string {
		"RADIO:OK\r\n"
		"FLASH:ERROR\r\n"
		"BACKUP:OK\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::commands::test parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_FALSE(parse_result);
}

TEST(cmd_test_parser_ut, cmd_test_backup_error)
{
	const std::string test_string {
		"RADIO:OK\r\n"
		"FLASH:OK\r\n"
		"BACKUP:ERROR\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::commands::test parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_FALSE(parse_result);
}

TEST(cmd_test_parser_ut, cmd_test_radio_wrong_token)
{
	const std::string test_string {
		" RADIO:ERROR\r\n"
		"FLASH:OK\r\n"
		"BACKUP:OK\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::commands::test parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_FALSE(parse_result);
}

TEST(cmd_test_parser_ut, cmd_test_radio_missing_token)
{
	const std::string test_string {
		"FLASH:OK\r\n"
		"BACKUP:OK\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::commands::test parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_FALSE(parse_result);
}

TEST(cmd_test_parser_ut, cmd_test_flash_wrong_token)
{
	const std::string test_string {
		"RADIO:OK\r\n"
		" FLASH:OK\r\n"
		"BACKUP:OK\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::commands::test parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_FALSE(parse_result);
}

TEST(cmd_test_parser_ut, cmd_test_flash_missing_token)
{
	const std::string test_string {
		"RADIO:OK\r\n"
		"BACKUP:OK\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::commands::test parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_FALSE(parse_result);
}

TEST(cmd_test_parser_ut, cmd_test_backup_wrong_token)
{
	const std::string test_string {
		"RADIO:OK\r\n"
		"FLASH:OK\r\n"
		" BACKUP:OK\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::commands::test parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_FALSE(parse_result);
}

TEST(cmd_test_parser_ut, cmd_test_backup_missing_token)
{
	const std::string test_string {
		"RADIO:OK\r\n"
		"FLASH:OK\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::commands::test parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_FALSE(parse_result);
}

//TODO: whole response tests with builders - needs to be refactored
TEST(cmd_test_parser_ut, test_command_ok)
{
	const std::string test_string {
		"COMMAND:TEST?\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::commands::test_command parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_TRUE(parse_result);
}

TEST(cmd_test_parser_ut, test_command_fail)
{
	const std::string test_string {
		"COMMAND:UKNOWN\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::commands::test_command parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_FALSE(parse_result);
}

TEST(cmd_test_parser_ut, cmd_test_algorithm_ok)
{
	const std::string test_string {
		"AT:START\r\n"
		"SOURCE:COMMAND\r\n"
		"COMMAND:TEST?\r\n"
		"STATUS:OK\r\n"
		"RADIO:OK\r\n"
		"FLASH:OK\r\n"
		"BACKUP:OK\r\n"
		"AT:STOP\r\n"
	};
	std::string_view test_string_view(test_string);
	auto parser_builder = aura::parser::commands::test::parser_builder();
	bool status = false;
	for (auto &parser : parser_builder) {
		status = parser->parse(test_string_view);
		if (!status) {
			break;
		}
	}
	EXPECT_TRUE(status);
}
