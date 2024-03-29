/*
 * cmd_xtal_correction_ut.cpp
 */

#include "commands/xtal_correction.hpp"
#include "gtest/gtest.h"

TEST(cmd_xtal_correction_ut, cmd_xtal_correction_positive_ok)
{
	const std::string test_value { "123456789 " };
	const std::string test_string {
		"VALUE: " +
		test_value +
		"\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::commands::xtal_correction parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_EQ(test_value, *parse_result);
	EXPECT_TRUE(parse_result);
}

TEST(cmd_xtal_correction_ut, cmd_xtal_correction_negative_ok)
{
	const std::string test_value { "-987654321 " };
	const std::string test_string {
		"VALUE: " +
		test_value +
		"\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::commands::xtal_correction parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_EQ(test_value, *parse_result);
	EXPECT_TRUE(parse_result);
}

TEST(cmd_xtal_correction_ut, cmdxtal_correction_wrong_token)
{
	const std::string test_string {
		"VALUI: 800000\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::commands::xtal_correction parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_FALSE(parse_result);
}

TEST(cmd_xtal_correction_ut, cmdxtal_correction_wrong_value_1)
{
	const std::string test_string {
		"VALUE: *800000\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::commands::xtal_correction parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_FALSE(parse_result);
}

TEST(cmd_xtal_correction_ut, cmdxtal_correction_wrong_value_2)
{
	const std::string test_string {
		"VALUE: 800000 S\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::commands::xtal_correction parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_FALSE(parse_result);
}

TEST(cmd_xtal_correction_ut, cmdxtal_correction_wrong_value_3)
{
	const std::string test_string {
		"VALUE: 80000A0\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::commands::xtal_correction parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_FALSE(parse_result);
}
