/*
 * address_ut.cpp
 */

#include "gtest/gtest.h"
#include "address.hpp"

TEST(address_ut, cmd_address_ok)
{
	const std::string test_address = "9CDEB60B";
	const std::string test_string {
		"ADDRESS: " +
		test_address +
		"\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::address parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_TRUE(parse_result);
	EXPECT_EQ(test_address, *parse_result);
}

TEST(address_ut, cmd_address_wrong_token)
{
	const std::string test_string {
		"ADDRES: 9CDEB60B\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::address parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_FALSE(parse_result);
}

TEST(address_ut, cmd_address_wrong_value_1)
{
	const std::string test_string {
		"ADDRESS: CDEB60B\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::address parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_FALSE(parse_result);
}

TEST(address_ut, cmd_address_wrong_value_2)
{
	const std::string test_string {
		"ADDRESS: AABBCCRF\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::address parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_FALSE(parse_result);
}

TEST(address_ut, cmd_address_wrong_value_3)
{
	const std::string test_string {
		"ADDRESS: AABBCCDDEE\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::address parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_FALSE(parse_result);
}
