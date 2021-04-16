/*
 * specific_command_parser_ut.cpp
 */

#include "gtest/gtest.h"
#include "parsers/commands/specific_command.hpp"

TEST(specific_command_parser_ut, specific_command_test_ok)
{
	const std::string command {
		"SPECIFIC_COMMAND"
	};
	const std::string test_string {
		"COMMAND:" +
		command +
		"\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::commands::specific_command parser_ut(command);
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_TRUE(parse_result);
}

TEST(specific_command_parser_ut, specific_command_test_fail)
{
	const std::string command {
		"SPECIFIC_COMMAND"
	};
	const std::string wrong_command {
		"WRONG_COMMAND"
	};
	const std::string test_string {
		"COMMAND:" +
		command +
		"\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::commands::specific_command parser_ut(wrong_command);
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_FALSE(parse_result);
}
