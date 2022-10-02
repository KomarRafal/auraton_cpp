/*
 * parser_ut.cpp
 */
#include <vector>
#include <string>

#include "gtest/gtest.h"
#include "parameter.hpp"
#include "parser.hpp"

TEST(parser_ut, parse_test)
{
	{
		const std::string test_string{
				"Parametr_1: some_1st_value\r\n"
				"Lorem ipsum dolor sit amet,\r\n"
				"consectetur adipiscing elit, sed do eiusmod\r\n"
				"tempor incididunt ut labore et dolore magna aliqua.\r\n"
				"Parametr 2: some 2nd_value\r\n"
				"some important message\r\n"
				"message_without_termination"
			};
		EXPECT_TRUE(aura::parser_legacy::parse(test_string, "Parameter 100").empty());
		EXPECT_TRUE(aura::parser_legacy::parse(test_string, "Parametr_1: ") == "some_1st_value");
		EXPECT_TRUE(aura::parser_legacy::parse(test_string, "Parametr 2: ") == "some 2nd_value");
		EXPECT_TRUE(aura::parser_legacy::parse(test_string, "some important", false) == "some important message");
		EXPECT_TRUE(aura::parser_legacy::parse(test_string, "message_without_termination", false) == "message_without_termination");
	}
	{
		const std::string test_string{
				"Lorem ipsum dolor sit amet,\r\n"
				"consectetur adipiscing elit, sed do eiusmod\r\n"
				"tempor incididunt ut labore et dolore magna aliqua.\r\n"
				"param_without: termination"
			};
		EXPECT_TRUE(aura::parser_legacy::parse(test_string, "param_without: ", true) == "termination");
	}
}

TEST(parser_ut, parse_get_next_parameter_test_ok)
{
	std::string test_string{
		"CODE: 32\r\n"
		"CHANNEL: 0\r\n"
		"FLAG OWN: 0\r\n"
		"FLAG WRITEABLE: 1\r\n"
		"VALUE: 178\r\n"
		"CODE: 33\r\n"
		"CHANNEL: 10\r\n"
		"FLAG OWN: 0\r\n"
		"FLAG WRITEABLE: 1\r\n"
		"VALUE: 220\r\n"
		"CODE: 1\r\n"
		"CHANNEL: 0\r\n"
		"FLAG OWN: 5\r\n"
		"FLAG WRITEABLE: 1\r\n"
		"VALUE: 1\r\n"
		"CODE: 101\r\n"
		"CHANNEL: 0\r\n"
		"FLAG OWN: 0\r\n"
		"FLAG WRITEABLE: 1\r\n"
		"VALUE: 1927\r\n"
		"CODE: 769\r\n"
		"CHANNEL: 0\r\n"
		"FLAG OWN: 0\r\n"
		"FLAG WRITEABLE: 1\r\n"
		"VALUE: -1234\r\n"
		"CODE: 256\r\n"
		"CHANNEL: 0\r\n"
		"FLAG OWN: 0\r\n"
		"FLAG WRITEABLE: 1\r\n"
		"VALUE: -3890"
	};
	const std::vector<aura::parameter> test_parameters = {
			{aura::parameter{32, 0, 0, 1, 178}},
			{aura::parameter{33, 10, 0, 1, 220}},
			{aura::parameter{1, 0, 5, 1, 1}},
			{aura::parameter{101, 0, 0, 1, 1927}},
			{aura::parameter{769, 0, 0, 1, -1234}},
			{aura::parameter{256, 0, 0, 1, -3890}},
	};

	std::vector<aura::parameter> read_parameters;

	aura::parameter parameter{0};
	while (aura::parser_legacy::get_next_parameter(test_string, parameter)) {
		read_parameters.push_back(parameter);
	}
	EXPECT_EQ(test_parameters, read_parameters);
}

TEST(parser_ut, parse_get_next_parameter_test_not_all_ok)
{
	std::string test_string{
		"PCODE: 34\r\n"
		"CODE: 32\r\n"
		"CHANNEL: 0\r\n"
		"VALUE: 178\r\n"
		"CODE: \r\n"
		"CODE: 33\r\n"
		"FLAG OWN: 0\r\n"
		"FLAG WRITEABLE: 1\r\n"
	};
	const std::vector<aura::parameter> test_parameters = {
			{aura::parameter{32, 0, 0, 0, 178}},
			{aura::parameter{33, 0, 0, 1, 0}},
	};

	std::vector<aura::parameter> read_parameters;

	aura::parameter parameter{0};
	while (aura::parser_legacy::get_next_parameter(test_string, parameter)) {
		read_parameters.push_back(parameter);
	}
	EXPECT_EQ(test_parameters, read_parameters);
}

TEST(parser_ut, parse_get_next_parameter_test_code_fail)
{
	std::string test_string{
		"PCODE: 34\r\n"
		"CHANNEL: 0\r\n"
		"FLAG OWN: 0\r\n"
		"FLAG WRITEABLE: 1\r\n"
		"VALUE: 178\r\n"
		"COD: 32\r\n"
		"CHANNEL: 0\r\n"
		"FLAG OWN: 0\r\n"
		"FLAG WRITEABLE: 1\r\n"
		"VALUE: 178\r\n"
		"CODE: \r\n"
		"CHANNEL: 0\r\n"
		"FLAG OWN: 0\r\n"
		"FLAG WRITEABLE: 1\r\n"
		"VALUE: 178\r\n"
	};

	aura::parameter parameter{0};
	EXPECT_FALSE(aura::parser_legacy::get_next_parameter(test_string, parameter));
}
