/*
 * specific_command_parser_ut.cpp
 */

#include "gtest/gtest.h"
#include "commands/specific_command.hpp"

const std::string test_payload {
	"Lorem ipsum dolor sit amet,\r\n"
	"consectetur adipiscing elit, sed do eiusmod"
	"tempor incididunt ut labore et dolore magna aliqua.\r\n"
};

TEST(specific_command_parser_ut, specific_command_test_ok)
{
	const std::string command {
		"SPECIFIC_COMMAND"
	};
	const std::string test_string {
		"COMMAND:" +
		command +
		"\r\n" +
		test_payload
	};
	std::string_view test_string_view(test_string);
	aura::parser::commands::specific_command parser_ut(command);
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_TRUE(parse_result);
	EXPECT_EQ(test_payload, static_cast<std::string>(test_string_view));
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
