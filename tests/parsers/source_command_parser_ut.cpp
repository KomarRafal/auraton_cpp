/*
 * source_command_parser_ut.cpp
 */

#include "gtest/gtest.h"
#include "parsers/source_command.hpp"

const std::string test_payload {
	"Lorem ipsum dolor sit amet,\r\n"
	"consectetur adipiscing elit, sed do eiusmod"
	"tempor incididunt ut labore et dolore magna aliqua.\r\n"
};

TEST(source_command_parser_ut, source_parse_test_ok)
{
	const std::string test_string {
		"SOURCE:COMMAND\r\n" +
		test_payload
	};
	std::string_view test_string_view(test_string);
	aura::parser::source_command parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_TRUE(parse_result);
	EXPECT_EQ(test_payload, static_cast<std::string>(test_string_view));
}

TEST(source_command_parser_ut, source_parse_test_wrong_token)
{
	const std::string test_string {
		"SOOURCE:COMMAND\r\n" +
		test_payload
	};
	std::string_view test_string_view(test_string);
	aura::parser::source_command parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_FALSE(parse_result);
}

TEST(source_command_parser_ut, source_parse_test_moved_token)
{
	const std::string test_string {
		" SOURCE:COMMAND\r\n" +
		test_payload
	};
	std::string_view test_string_view(test_string);
	aura::parser::source_command parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_FALSE(parse_result);
}

TEST(source_command_parser_ut, source_parse_test_wrong_source)
{
	const std::string test_string {
		"SOURCE:HEAVEN\r\n" +
		test_payload
	};
	std::string_view test_string_view(test_string);
	aura::parser::source_command parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_FALSE(parse_result);
}

TEST(source_command_parser_ut, source_parse_test_eol_missing)
{
	const std::string test_string {
		"SOURCE:COMMAND"
	};
	std::string_view test_string_view(test_string);
	aura::parser::source_command parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_FALSE(parse_result);
}

TEST(source_command_parser_ut, source_parse_test_payload_missing)
{
	const std::string test_string {
		"SOURCE:COMMAND\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::source_command parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_TRUE(parse_result);
	EXPECT_TRUE(test_string_view.empty());
}
