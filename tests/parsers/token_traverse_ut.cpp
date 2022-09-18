/*
 * token_traverse_ut.cpp
 */

#include <vector>
#include "token_traverse.hpp"
#include "gtest/gtest.h"

TEST(token_traverse_ut, token_traverse_one_token_ok)
{
	const std::string token = "ID:";
	const std::string token_traverse_string{
		token + " 20\r\n"
		"ADDRESS: 456223DA\r\n"
		"PCODE: 3005\r\n"
		"FVER: 1.5\r\n"
		"HVER: 1\r\n"
		"MANCODE: 30\r\n"
	};
	std::string_view token_traverse_string_view(token_traverse_string);
	aura::parser::token_traverse parser_ut(token);
	auto parse_result = parser_ut.parse(token_traverse_string_view);
	ASSERT_TRUE(static_cast<bool>(parse_result));
	EXPECT_EQ(*parse_result, token_traverse_string);
	EXPECT_TRUE(token_traverse_string_view.empty());
	parse_result = parser_ut.parse(token_traverse_string_view);
	EXPECT_FALSE(static_cast<bool>(parse_result));
}

TEST(token_traverse_ut, token_traverse_many_tokens_ok)
{
	const std::string token = "ID:";
	const std::vector<std::string> devices{
		{
			token + " 45\r\n"
			"Lorem ipsum dolor sit amet,\r\n"
			"consectetur adipiscing elit, sed do eiusmod\r\n"
			"tempor incididunt ut labore et dolore magna aliqua.\r\n"
			"OK\r\n"
			"\n\r\n\r\n\r PADDING"
		},
		{
			token + " 1\r\n"
			"ADDRESS: 90FC4F9B\r\n"
			"PCODE: 300d\r\n"
			"FVER: 1.1\r\n"
			"HVER: 7\r\n"
			"MANCODE: 30\r\n"
		},
		{
			token + " 20\r\n"
			"ADDRESS: 456223DA\r\n"
			"PCODE: 3005\r\n"
			"FVER: 1.5\r\n"
			"HVER: 1\r\n"
			"MANCODE: 30\r\n"
		}
	};
	const std::string token_traverse_string{
		devices[0] +
		devices[1] +
		devices[2]
	};
	std::vector<std::string>::size_type device_found = 0;
	std::string_view token_traverse_string_view(token_traverse_string);
	aura::parser::token_traverse parser_ut(token);
	auto parse_result = parser_ut.parse(token_traverse_string_view);
	while (parse_result.has_value()) {
		EXPECT_EQ(*parse_result, devices[device_found]);
		parse_result = parser_ut.parse(token_traverse_string_view);
		device_found++;
	}
	EXPECT_EQ(device_found, devices.size());
}

TEST(token_traverse_ut, token_traverse_error)
{
	const std::string token = "ID:";
	const std::string error_test_string{
			"Lorem ipsum dolor sit amet,\r\n"
			"consectetur adipiscing elit, sed do eiusmod\r\n"
			"tempor incididunt ut labore et dolore magna aliqua.\r\n"
		};
	std::string_view token_traverse_string_view(error_test_string);
	aura::parser::token_traverse parser_ut(token);
	auto parse_result = parser_ut.parse(token_traverse_string_view);
	EXPECT_FALSE(static_cast<bool>(parse_result));
	EXPECT_EQ(error_test_string, static_cast<std::string>(token_traverse_string_view));
}

TEST(token_traverse_ut, token_traverse_empty)
{
	const std::string token = "ID:";
	const std::string empty_test_string{};
	std::string_view token_traverse_string_view(empty_test_string);
	aura::parser::token_traverse parser_ut(token);
	auto parse_result = parser_ut.parse(token_traverse_string_view);
	EXPECT_FALSE(static_cast<bool>(parse_result));
	EXPECT_TRUE(token_traverse_string_view.empty());
}
