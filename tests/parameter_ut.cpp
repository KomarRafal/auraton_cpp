/*
 * parameter_ut.cpp
 */
#include <string_view>
#include <vector>

#include "parameter.hpp"
#include "gtest/gtest.h"

TEST(parameter_ut, get_next_parameter_test_ok)
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
	std::string_view test_string_view{test_string};
	for (auto parameter = aura::parameter::get_next_parameter(test_string_view);
			parameter.has_value();
			parameter = aura::parameter::get_next_parameter(test_string_view)) {
		read_parameters.push_back(*parameter);
	}
	EXPECT_EQ(test_parameters, read_parameters);
}

TEST(parameter_ut, get_next_parameter_test_not_all_ok)
{
	std::string test_string{
		"CODE: 32\r\n"
		"CHANNEL: X\r\n"
		"FLAG OWN: 0\r\n"
		"FLAG WRITEABLE: 1\r\n"
		"VALUE: 178\r\n"
		"CODE: 33\r\n"
		"CHANNEL: 10\r\n"
		"FLAG OWN: 0\r\n"
		"FLAG WRITEABLE: 1\r\n"
		"VALUE: 220\r\n"
		"CODE: 1\r\n"
		"CHANNEL: a\r\n"
		"FLAG OWN: 5\r\n"
		"FLAG WRITEABLE: 1\r\n"
		"VALUE: 1\r\n"
		"CODE: 101\r\n"
		"CHANNEL: 0\r\n"
		"FLAG OWNX: 0\r\n"
		"FLAG WRITEABLE: 1\r\n"
		"VALUE: 1927\r\n"
		"CODE: 769\r\n"
		"CHANNEL: 0\r\n"
		"FLAG OWN: 0\r\n"
		"FLAG WRITEABLE:\r\n"
		"VALUE: -1234\r\n"
		"CODE: 256\r\n"
		"CHANNEL: 0\r\n"
		"FLAG OWN: 0\r\n"
		"FLAG WRITEABLE: 1\r\n"
	};
	const std::vector<aura::parameter> test_parameters = {
			{aura::parameter{33, 10, 0, 1, 220}},
	};

	std::vector<aura::parameter> read_parameters;
	std::string_view test_string_view{test_string};
	for (auto parameter = aura::parameter::get_next_parameter(test_string_view);
			parameter.has_value();
			parameter = aura::parameter::get_next_parameter(test_string_view)) {
		read_parameters.push_back(*parameter);
	}
	EXPECT_EQ(test_parameters, read_parameters);
}

TEST(parameter_ut, get_next_parameter_test_code_fail)
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

	std::string_view test_string_view{test_string};
	auto parameter = aura::parameter::get_next_parameter(test_string_view);
	EXPECT_FALSE(parameter.has_value());
}

