/*
 * cmd_test_ut.cpp
 */

#include "commands/specific_command.hpp"
#include "parser_executor.hpp"
#include "commands/test.hpp"
#include "gtest/gtest.h"

TEST(cmd_test_ut, cmd_test_test_simple_ok)
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

TEST(cmd_test_ut, cmd_test_radio_error)
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

TEST(cmd_test_ut, cmd_test_flash_error)
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

TEST(cmd_test_ut, cmd_test_backup_error)
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

TEST(cmd_test_ut, cmd_test_radio_wrong_token)
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

TEST(cmd_test_ut, cmd_test_radio_missing_token)
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

TEST(cmd_test_ut, cmd_test_flash_wrong_token)
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

TEST(cmd_test_ut, cmd_test_flash_missing_token)
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

TEST(cmd_test_ut, cmd_test_backup_wrong_token)
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

TEST(cmd_test_ut, cmd_test_backup_missing_token)
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

TEST(cmd_test_ut, cmd_test_algorithm_ok)
{
	const std::string test_string {
		"AT:START\r\n"
		"SOURCE:COMMAND\r\n"
		/* TODO: remove whitespace after FW fix */
		"COMMAND: TEST?\r\n"
		"STATUS:OK\r\n"
		"RADIO:OK\r\n"
		"FLASH:OK\r\n"
		"BACKUP:OK\r\n"
		"AT:STOP\r\n"
	};
	std::string_view test_string_view(test_string);
	const auto status = aura::parser::parser_executor::execute(
			test_string_view,
			aura::parser::commands::test_builder::build());
	EXPECT_TRUE(static_cast<bool>(status));
}

