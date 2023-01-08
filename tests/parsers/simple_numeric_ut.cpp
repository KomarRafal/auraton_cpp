/*
 * simple_numeric_ut.cpp
 */

#include "simple_numeric.hpp"
#include "gtest/gtest.h"

TEST(simple_numeric_ut, simple_numeric_ok)
{
	const std::string test_number = "45";
	const std::string test_token = "ID: ";
	const std::string test_string {
		test_token +
		test_number +
		"\r\n"
		"Some device parameters\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::simple_numeric parser_ut(test_token);
	const auto parse_result = parser_ut.parse(test_string_view);
	ASSERT_TRUE(parse_result);
	EXPECT_EQ(test_number, *parse_result);
}

TEST(simple_numeric_ut, simple_numeric_wrong_token)
{
	const std::string test_token = "ID: ";
	const std::string test_string {
		"IX: 45\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::simple_numeric parser_ut(test_token);
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_FALSE(parse_result);
}

TEST(simple_numeric_ut, simple_numeric_wrong_value)
{
	const std::string test_token = "ID: ";
	const std::string test_string {
		"ID: AB\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::simple_numeric parser_ut(test_token);
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_FALSE(parse_result);
}

TEST(simple_numeric_ut, simple_numeric_empty)
{
	const std::string test_token = "ID: ";
	const std::string test_string {
		"ID: \r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::simple_numeric parser_ut(test_token);
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_FALSE(parse_result);
}

TEST(simple_numeric_ut, simple_numeric_zero)
{
	const std::string test_token = "ID: ";
	const std::string test_string {
		"ID: 0\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::simple_numeric parser_ut(test_token);
	const auto parse_result = parser_ut.parse(test_string_view);
	ASSERT_TRUE(parse_result);
	EXPECT_EQ("0", *parse_result);
}
