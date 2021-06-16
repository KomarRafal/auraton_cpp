/*
 * cmd_address_ut.cpp
 */

#include "commands/address.hpp"
#include "gtest/gtest.h"

TEST(cmd_address_ut, cmd_address_ok)
{
	const std::string test_string {
		"ADDRESS: 9CDEB60B\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::commands::address parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_TRUE(parse_result);
}

TEST(cmd_address_ut, cmd_address_wrong_token)
{
	const std::string test_string {
		"ADDRES: 9CDEB60B\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::commands::address parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_FALSE(parse_result);
}

TEST(cmd_address_ut, cmd_address_wrong_value)
{
	const std::string test_string {
		"ADDRESS: CDEB60B\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::commands::address parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_FALSE(parse_result);
}

