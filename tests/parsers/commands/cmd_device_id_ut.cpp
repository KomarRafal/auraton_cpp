/*
 * cmd_device_id_ut.cpp
 */

#include "commands/device_id.hpp"
#include "gtest/gtest.h"

TEST(cmd_device_id_ut, cmd_device_id_ok)
{
	const std::string test_device_id = "45";
	const std::string test_string {
		"ID: " +
		test_device_id +
		"\r\n"
		"Some device parameters\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::commands::device_id parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	ASSERT_TRUE(parse_result);
	EXPECT_EQ(test_device_id, *parse_result);
}

TEST(cmd_device_id_ut, cmd_device_id_wrong_token)
{
	const std::string test_string {
		"IX: 45\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::commands::device_id parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_FALSE(parse_result);
}

TEST(cmd_device_id_ut, cmd_device_id_wrong_value)
{
	const std::string test_string {
		"ID: AB\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::commands::device_id parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_FALSE(parse_result);
}

TEST(cmd_device_id_ut, cmd_device_id_empty)
{
	const std::string test_string {
		"ID: \r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::commands::device_id parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_FALSE(parse_result);
}
