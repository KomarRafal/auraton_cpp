/*
 * command_parser_ut.cpp
 */

#include "gtest/gtest.h"
#include "parser/command_parser.hpp"

TEST(command_parser_ut, command_parser_test_simple_ok)
{
	const std::string test_payload {
		"Lorem ipsum dolor sit amet,\r\n"
		"consectetur adipiscing elit, sed do eiusmod"
		"tempor incididunt ut labore et dolore magna aliqua.\r\n"
	};

	const std::string test_string {
		"AT:START\r\n"
		"SOURCE:COMMAND\r\n"
		"STATUS:OK\r\n" +
		test_payload +
		"AT:STOP\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::command_parser parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_TRUE(parse_result);
	EXPECT_EQ(test_payload, static_cast<std::string>(test_string_view));
}

TEST(command_parser_ut, command_parser_test_at_fail)
{
	const std::string test_payload {
		"Lorem ipsum dolor sit amet,\r\n"
		"consectetur adipiscing elit, sed do eiusmod"
		"tempor incididunt ut labore et dolore magna aliqua.\r\n"
	};

	const std::string test_string {
		"AT:S\r\n"
		"SOURCE:COMMAND\r\n"
		"STATUS:OK\r\n" +
		test_payload +
		"AT:STOP\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::command_parser parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_FALSE(parse_result);
}

TEST(command_parser_ut, command_parser_test_source_event)
{
	const std::string test_payload {
		"Lorem ipsum dolor sit amet,\r\n"
		"consectetur adipiscing elit, sed do eiusmod"
		"tempor incididunt ut labore et dolore magna aliqua.\r\n"
	};

	const std::string test_string {
		"AT:START\r\n"
		"SOURCE:EVENT\r\n"
		"STATUS:OK\r\n" +
		test_payload +
		"AT:STOP\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::command_parser parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_FALSE(parse_result);
}

TEST(command_parser_ut, command_parser_test_status_error)
{
	const std::string test_payload {
		"Lorem ipsum dolor sit amet,\r\n"
		"consectetur adipiscing elit, sed do eiusmod"
		"tempor incididunt ut labore et dolore magna aliqua.\r\n"
	};

	const std::string test_string {
		"AT:START\r\n"
		"SOURCE:COMMAND\r\n"
		"STATUS:ERROR\r\n" +
		test_payload +
		"AT:STOP\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::command_parser parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_FALSE(parse_result);
}
