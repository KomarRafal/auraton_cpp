/*
 * status_parser_ut.cpp
 */

#include "gtest/gtest.h"
#include "parser/status.hpp"

const std::string test_payload {
	"Lorem ipsum dolor sit amet,\r\n"
	"consectetur adipiscing elit, sed do eiusmod"
	"tempor incididunt ut labore et dolore magna aliqua.\r\n"
};

TEST(status_parser_ut, status_parse_test_ok_ok)
{
	const std::string test_string {
		"STATUS:OK\r\n" +
		test_payload
	};
	std::string_view test_string_view(test_string);
	aura::parser::status parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_TRUE(parse_result);
	EXPECT_EQ(test_payload, static_cast<std::string>(test_string_view));
}

TEST(status_parser_ut, status_parse_test_error_ok)
{
	const std::string test_string {
		"STATUS:ERROR\r\n" +
		test_payload
	};
	std::string_view test_string_view(test_string);
	aura::parser::status parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_FALSE(parse_result);
	EXPECT_EQ(test_payload, static_cast<std::string>(test_string_view));
}

TEST(status_parser_ut, status_parse_test_ok_wrong_token)
{
	const std::string test_string {
		"SOOURCE:OK\r\n" +
		test_payload
	};
	std::string_view test_string_view(test_string);
	aura::parser::status parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_FALSE(parse_result);
}

TEST(status_parser_ut, status_parse_test_ok_moved_token)
{
	const std::string test_string {
		" STATUS:OK\r\n" +
		test_payload
	};
	std::string_view test_string_view(test_string);
	aura::parser::status parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_FALSE(parse_result);
}

TEST(status_parser_ut, status_parse_test_ok_wrong_source)
{
	const std::string test_string {
		"STATUS:UKNOWN\r\n" +
		test_payload
	};
	std::string_view test_string_view(test_string);
	aura::parser::status parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_FALSE(parse_result);
}

TEST(status_parser_ut, status_parse_test_ok_eol_missing)
{
	const std::string test_string {
		"STATUS:OK"
	};
	std::string_view test_string_view(test_string);
	aura::parser::status parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_FALSE(parse_result);
}

TEST(status_parser_ut, status_parse_test_ok_payload_missing)
{
	const std::string test_string {
		"STATUS:OK\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::status parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_TRUE(parse_result);
	EXPECT_TRUE(test_string_view.empty());
}
