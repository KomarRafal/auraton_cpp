/*
 * version_ut.cpp
 */

//#include "commands/specific_command.hpp"
#include "parser_executor.hpp"
#include "gtest/gtest.h"
#include "version.hpp"

TEST(version_ut, cmd_pcode_version_ok)
{
	const std::string test_value{ "300c" };
	const std::string test_string {
		"PCODE: " +
		test_value +
		"\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::version_pcode parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_EQ(test_value, *parse_result);
	EXPECT_TRUE(parse_result);
}

TEST(version_ut, cmd_pcode_version_fail)
{
	const std::string test_string {
		"PCODE: \r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::version_pcode parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_FALSE(parse_result);
}

TEST(version_ut, cmd_pcode_version_token_fail)
{
	const std::string test_string {
		"PODE: 300c\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::version_pcode parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_FALSE(parse_result);
}

TEST(version_ut, cmd_fw_version_ok)
{
	const std::string test_value{ "2.4" };
	const std::string test_string {
		"FVER: " +
		test_value +
		"\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::version_fw_version parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_EQ(test_value, *parse_result);
	EXPECT_TRUE(parse_result);
}

TEST(version_ut, cmd_fw_version_fail)
{
	const std::string test_string {
		"FVER: \r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::version_fw_version parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_FALSE(parse_result);
}

TEST(version_ut, cmd_fw_version_token_fail)
{
	const std::string test_string {
		"HVER: 2.4\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::version_fw_version parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_FALSE(parse_result);
}

TEST(version_ut, cmd_hw_version_ok)
{
	const std::string test_value{ "1.0" };
	const std::string test_string {
		"HVER: " +
		test_value +
		"\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::version_hw_version parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_EQ(test_value, *parse_result);
	EXPECT_TRUE(parse_result);
}

TEST(version_ut, cmd_hw_version_fail)
{
	const std::string test_string {
		"HVER: \r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::version_hw_version parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_FALSE(parse_result);
}

TEST(version_ut, cmd_hw_version_token_fail)
{
	const std::string test_string {
		"FVER: 1.0\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::version_hw_version parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_FALSE(parse_result);
}

TEST(version_ut, cmd_man_code_version_ok)
{
	const std::string test_value{ "30" };
	const std::string test_string {
		"MANCODE: " +
		test_value +
		"\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::version_man_code parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_EQ(test_value, *parse_result);
	EXPECT_TRUE(parse_result);
}

TEST(version_ut, cmd_man_code_version_fail)
{
	const std::string test_string {
		"MANCODE: \r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::version_man_code parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_FALSE(parse_result);
}

TEST(version_ut, cmd_man_code_version_token_fail)
{
	const std::string test_string {
		"MANODE: 30\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::version_man_code parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_FALSE(parse_result);
}
