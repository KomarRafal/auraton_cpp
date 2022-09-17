/*
 * cmd_device_parameter_ut.cpp
 */

#include <vector>
#include "commands/device_parameter.hpp"
#include "gtest/gtest.h"

TEST(cmd_device_parameter_ut, cmd_device_parameter_ok)
{
	const std::string device_parameter_string{
		"CODE: 33\r\n"
		"CHANNEL: 0\r\n"
		"FLAG OWN: 0\r\n"
		"FLAG WRITEABLE: 1\r\n"
		"VALUE: 0\r\n"
	};
	std::string_view device_parameter_string_view(device_parameter_string);
	aura::parser::commands::device_parameter parser_ut;
	auto parse_result = parser_ut.parse(device_parameter_string_view);
	ASSERT_TRUE(static_cast<bool>(parse_result));
	EXPECT_EQ(*parse_result, device_parameter_string);
	EXPECT_TRUE(device_parameter_string_view.empty());
	parse_result = parser_ut.parse(device_parameter_string_view);
	EXPECT_FALSE(static_cast<bool>(parse_result));
}

TEST(cmd_device_parameter_ut, cmd_device_parameter_many_parameters_ok)
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
			"CODE: 1\r\n"
			"CHANNEL: 0\r\n"
			"FLAG OWN: 0\r\n"
			"FLAG WRITEABLE: 1\r\n"
			"VALUE: 0\r\n"
		},
		{
			"CODE: 20\r\n"
			"CHANNEL: 1\r\n"
			"FLAG OWN: 0\r\n"
			"FLAG WRITEABLE: 0\r\n"
			"VALUE: 230\r\n"
		}
	};
	const std::string device_parameter_string{
		parameters[0] +
		parameters[1] +
		parameters[2]
	};
	std::vector<std::string>::size_type parameters_found = 0;
	std::string_view device_parameter_string_view(device_parameter_string);
	aura::parser::commands::device_parameter parser_ut;
	auto parse_result = parser_ut.parse(device_parameter_string_view);
	while (parse_result.has_value()) {
		EXPECT_EQ(*parse_result, parameters[parameters_found]);
		parse_result = parser_ut.parse(device_parameter_string_view);
		parameters_found++;
	}
	EXPECT_EQ(parameters_found, parameters.size());
}

TEST(cmd_device_parameter_ut, cmd_device_parameter_error)
{
	const std::string error_test_string{
			"Lorem ipsum dolor sit amet,\r\n"
			"consectetur adipiscing elit, sed do eiusmod\r\n"
			"tempor incididunt ut labore et dolore magna aliqua.\r\n"
		};
	std::string_view device_parameter_string_view(error_test_string);
	aura::parser::commands::device_parameter parser_ut;
	auto parse_result = parser_ut.parse(device_parameter_string_view);
	EXPECT_FALSE(static_cast<bool>(parse_result));
	EXPECT_EQ(error_test_string, static_cast<std::string>(device_parameter_string_view));
}

TEST(cmd_device_parameter_ut, cmd_device_parameter_empty)
{
	const std::string empty_test_string{};
	std::string_view device_parameter_string_view(empty_test_string);
	aura::parser::commands::device_parameter parser_ut;
	auto parse_result = parser_ut.parse(device_parameter_string_view);
	EXPECT_FALSE(static_cast<bool>(parse_result));
	EXPECT_TRUE(device_parameter_string_view.empty());
}
