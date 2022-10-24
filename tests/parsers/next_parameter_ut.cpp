/*
 * next_parameter_ut.cpp
 */

#include <vector>
#include "next_parameter.hpp"
#include "gtest/gtest.h"

TEST(next_parameter_ut, next_parameter_one_token_ok)
{
	const std::string parameter_string{
		"CODE: 101\r\n"
		"CHANNEL: 0\r\n"
		"FLAG OWN: 0\r\n"
		"FLAG WRITEABLE: 0\r\n"
		"VALUE: 1979\r\n"
	};
	std::string_view parameter_string_view{parameter_string};
	aura::parser::next_parameter parser_ut;
	auto parse_result = parser_ut.parse(parameter_string_view);
	ASSERT_TRUE(static_cast<bool>(parse_result));
	EXPECT_EQ(*parse_result, parameter_string);
	EXPECT_TRUE(parameter_string_view.empty());
	parse_result = parser_ut.parse(parameter_string_view);
	EXPECT_FALSE(static_cast<bool>(parse_result));
}

TEST(next_parameter_ut, next_parameter_many_tokens_ok)
{
	const std::vector<std::string> parameters{
		{
			"CODE: 45\r\n"
			"Lorem ipsum dolor sit amet,\r\n"
			"consectetur adipiscing elit, sed do eiusmod\r\n"
			"tempor incididunt ut labore et dolore magna aliqua.\r\n"
			"OK\r\n"
			"\n\r\n\r\n\r PADDING"
		},
		{
			"CODE: 101\r\n"
			"CHANNEL: 0\r\n"
			"FLAG OWN: 0\r\n"
			"FLAG WRITEABLE: 0\r\n"
			"VALUE: 1979\r\n"
		},
		{
			"CODE: 256\r\n"
			"CHANNEL: 0\r\n"
			"FLAG OWN: 0\r\n"
			"FLAG WRITEABLE: 1\r\n"
			"VALUE: 0\r\n"
		}
	};
	const std::string parameter_string{
		parameters[0] +
		parameters[1] +
		parameters[2]
	};
	std::vector<std::string>::size_type parameters_found = 0;
	std::string_view parameter_string_view(parameter_string);
	aura::parser::next_parameter parser_ut;
	auto parse_result = parser_ut.parse(parameter_string_view);
	while (parse_result.has_value()) {
		EXPECT_EQ(*parse_result, parameters[parameters_found]);
		parse_result = parser_ut.parse(parameter_string_view);
		parameters_found++;
	}
	EXPECT_EQ(parameters_found, parameters.size());
}
