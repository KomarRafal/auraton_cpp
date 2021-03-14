/*
 * cmd_test_parser_ut.cpp
 */

#include "gtest/gtest.h"
#include "parser/cmd_test.hpp"

TEST(cmd_test_parser_ut, cmd_test_test_simple_ok)
{
	const std::string test_string {
		"RADIO:OK\r\n"
		"FLASH:OK\r\n"
		"BACKUP:OK\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::cmd_test parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_TRUE(parse_result);
	EXPECT_EQ(parse_result.value(), aura::parser::cmd_test_type::TEST_OK);
}

TEST(cmd_test_parser_ut, cmd_test_radio_error)
{
	const std::string test_string {
		"RADIO:ERROR\r\n"
		"FLASH:OK\r\n"
		"BACKUP:OK\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::cmd_test parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_TRUE(parse_result);
	EXPECT_EQ(parse_result.value(), aura::parser::cmd_test_type::RADIO_ERROR);
}

TEST(cmd_test_parser_ut, cmd_test_flash_error)
{
	const std::string test_string {
		"RADIO:OK\r\n"
		"FLASH:ERROR\r\n"
		"BACKUP:OK\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::cmd_test parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_TRUE(parse_result);
	EXPECT_EQ(parse_result.value(), aura::parser::cmd_test_type::FLASH_ERROR);
}

TEST(cmd_test_parser_ut, cmd_test_backup_error)
{
	const std::string test_string {
		"RADIO:OK\r\n"
		"FLASH:OK\r\n"
		"BACKUP:ERROR\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::cmd_test parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_TRUE(parse_result);
	EXPECT_EQ(parse_result.value(), aura::parser::cmd_test_type::BACKUP_ERROR);
}

TEST(cmd_test_parser_ut, cmd_test_radio_wrong_token)
{
	const std::string test_string {
		" RADIO:ERROR\r\n"
		"FLASH:OK\r\n"
		"BACKUP:OK\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::cmd_test parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_TRUE(parse_result);
	EXPECT_EQ(parse_result.value(), aura::parser::cmd_test_type::RADIO_ERROR);
}

TEST(cmd_test_parser_ut, cmd_test_radio_missing_token)
{
	const std::string test_string {
		"FLASH:OK\r\n"
		"BACKUP:OK\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::cmd_test parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_TRUE(parse_result);
	EXPECT_EQ(parse_result.value(), aura::parser::cmd_test_type::RADIO_ERROR);
}

TEST(cmd_test_parser_ut, cmd_test_flash_wrong_token)
{
	const std::string test_string {
		"RADIO:OK\r\n"
		" FLASH:OK\r\n"
		"BACKUP:OK\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::cmd_test parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_TRUE(parse_result);
	EXPECT_EQ(parse_result.value(), aura::parser::cmd_test_type::FLASH_ERROR);
}

TEST(cmd_test_parser_ut, cmd_test_flash_missing_token)
{
	const std::string test_string {
		"RADIO:OK\r\n"
		"BACKUP:OK\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::cmd_test parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_TRUE(parse_result);
	EXPECT_EQ(parse_result.value(), aura::parser::cmd_test_type::FLASH_ERROR);
}

TEST(cmd_test_parser_ut, cmd_test_backup_wrong_token)
{
	const std::string test_string {
		"RADIO:OK\r\n"
		"FLASH:OK\r\n"
		" BACKUP:OK\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::cmd_test parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_TRUE(parse_result);
	EXPECT_EQ(parse_result.value(), aura::parser::cmd_test_type::BACKUP_ERROR);
}

TEST(cmd_test_parser_ut, cmd_test_backup_missing_token)
{
	const std::string test_string {
		"RADIO:OK\r\n"
		"FLASH:OK\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::cmd_test parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_TRUE(parse_result);
	EXPECT_EQ(parse_result.value(), aura::parser::cmd_test_type::BACKUP_ERROR);
}
