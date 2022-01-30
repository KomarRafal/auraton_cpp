/*
 * clear_parser_ut.cpp
 */

#include "parser_executor.hpp"
#include "gtest/gtest.h"
#include "clear.hpp"

TEST(clear_parser_ut, minus_1_to_101_range)
{
	const int8_t start_test = -1;
	const int8_t end_test = 101;
	const std::string clear_test_token {
		"PROGRESS:"
	};

	for (int8_t percent = start_test; percent <= end_test; percent++) {
		const auto test_string = clear_test_token + std::to_string(percent) + "\r\n";
		std::string_view test_string_view(test_string);
		aura::parser::clear parser_ut;
		const auto parse_result = parser_ut.parse(test_string_view);
		if (percent < 1 || percent > 100) {
			EXPECT_FALSE(parse_result);
		}
		else {
			EXPECT_EQ(parse_result.value_or(""), std::to_string(percent));
			EXPECT_TRUE(parse_result);
		}
	}
}

TEST(clear_parser_ut, not_a_number)
{
	const std::string test_string {
		"PROGRESS:abc\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::clear parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_FALSE(parse_result);
}

TEST(clear_parser_ut, empty)
{
	const std::string test_string {
		"PROGRESS:\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::clear parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_FALSE(parse_result);
}

TEST(clear_parser_ut, mixed_prefix_letter)
{
	const std::string test_string {
		"PROGRESS:a12\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::clear parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_FALSE(parse_result);
}

TEST(clear_parser_ut, mixed_postfix_letter)
{
	const std::string test_string {
		"PROGRESS:23b\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::clear parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_FALSE(parse_result);
}


TEST(clear_parser_ut, builder_ok)
{
	const std::string test_value = "17";
	const std::string test_string {
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:" + test_value + "\r\n"
		"AT:STOP\r\n"
	};
	std::string_view test_string_view(test_string);
	const auto status = aura::parser::parser_executor::execute(
			test_string_view,
			aura::parser::clear_builder::build());
	EXPECT_TRUE(static_cast<bool>(status));
	EXPECT_EQ(status.value_or(""), test_value);
}

