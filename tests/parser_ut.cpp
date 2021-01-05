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
		EXPECT_TRUE(aura::parser::parse(test_string, "Parameter 100").empty());
		EXPECT_TRUE(aura::parser::parse(test_string, "Parametr_1: ") == "some_1st_value");
		EXPECT_TRUE(aura::parser::parse(test_string, "Parametr 2: ") == "some 2nd_value");
		EXPECT_TRUE(aura::parser::parse(test_string, "some important", false) == "some important message");
		EXPECT_TRUE(aura::parser::parse(test_string, "message_without_termination", false) == "message_without_termination");
	}
	{
		const std::string test_string{
				"Lorem ipsum dolor sit amet,\r\n"
				"consectetur adipiscing elit, sed do eiusmod\r\n"
				"tempor incididunt ut labore et dolore magna aliqua.\r\n"
				"param_without: termination"
			};
		EXPECT_TRUE(aura::parser::parse(test_string, "param_without: ", true) == "termination");
	}
}

TEST(parser_ut, parse_device_list_test)
{
	const std::string ok_test_string{
			"ID: 45\r\n"
			"Lorem ipsum dolor sit amet,\r\n"
			"consectetur adipiscing elit, sed do eiusmod\r\n"
			"tempor incididunt ut labore et dolore magna aliqua.\r\n"
			"OK\r\n"
			"\n\r\n\r\n\r PADDING"
			"ID: 1\r\n"
			"ADDRESS: 90FC4F9B\r\n"
			"PCODE: 300d\r\n"
			"FVER: 1.1\r\n"
			"HVER: 7\r\n"
			"MANCODE: 30\r\n"
			"ID: 20\r\n"
			"ADDRESS: 456223DA\r\n"
			"PCODE: 3005\r\n"
			"FVER: 1.5\r\n"
			"HVER: 1\r\n"
			"MANCODE: 30\r\n"
		};
	const aura::parser::device_list_t ok_devices{
			{ 45,
			"Lorem ipsum dolor sit amet,\r\n"
			"consectetur adipiscing elit, sed do eiusmod\r\n"
			"tempor incididunt ut labore et dolore magna aliqua.\r\n"
			"OK\r\n"
			"\n\r\n\r\n\r PADDING"},
			{ 1,
			"ADDRESS: 90FC4F9B\r\n"
			"PCODE: 300d\r\n"
			"FVER: 1.1\r\n"
			"HVER: 7\r\n"
			"MANCODE: 30\r\n"},
			{ 20, "ADDRESS: 456223DA\r\n"
			"PCODE: 3005\r\n"
			"FVER: 1.5\r\n"
			"HVER: 1\r\n"
			"MANCODE: 30\r\n"} };
	const std::string error_test_string{
			"Lorem ipsum dolor sit amet,\r\n"
			"consectetur adipiscing elit, sed do eiusmod\r\n"
			"tempor incididunt ut labore et dolore magna aliqua.\r\n"
		};
	const std::string duplicate_id_test_string{
			"ID: 28\r\n"
			"ADDRESS: 90FC4F9B\r\n"
			"PCODE: 300d\r\n"
			"FVER: 1.1\r\n"
			"HVER: 7\r\n"
			"MANCODE: 30\r\n"
			"ID: 28\r\n"
			"ADDRESS: 456223DA\r\n"
			"PCODE: 3005\r\n"
			"FVER: 1.5\r\n"
			"HVER: 1\r\n"
			"MANCODE: 30"
		};
	const int32_t device_dup_key = 28;
	const std::string device_dup{
			"ADDRESS: 90FC4F9B\r\n"
			"PCODE: 300d\r\n"
			"FVER: 1.1\r\n"
			"HVER: 7\r\n"
			"MANCODE: 30\r\n"
			"ADDRESS: 456223DA\r\n"
			"PCODE: 3005\r\n"
			"FVER: 1.5\r\n"
			"HVER: 1\r\n"
			"MANCODE: 30"
		};
	const auto device_list_ok = aura::parser::parse_device_list(ok_test_string);
	const auto device_list_error = aura::parser::parse_device_list(error_test_string);
	const auto device_list_duplicate_id = aura::parser::parse_device_list(duplicate_id_test_string);

	EXPECT_EQ(device_list_ok.size(), ok_devices.size());
	for (auto dev : ok_devices) {
		const auto key = dev.first;
		const auto ok_it = device_list_ok.find(key);
		ASSERT_NE(ok_it, device_list_ok.end()) << "Missing device ID: " << key << std::endl;
		EXPECT_EQ(dev.second, ok_it->second);
	}
	EXPECT_EQ(device_list_error.size(), 0);
	EXPECT_EQ(device_list_duplicate_id.size(), 1);
	const auto ok_it_dup = device_list_duplicate_id.find(device_dup_key);
	ASSERT_NE(ok_it_dup, device_list_duplicate_id.end()) << "Missing device ID: " << device_dup_key << std::endl;
	EXPECT_EQ(device_dup, ok_it_dup->second);
}

TEST(parser_ut, check_result_test)
{
	const std::string terminated_ok_test_string{
				"Lorem ipsum dolor sit amet,\r\n"
				"consectetur adipiscing elit, sed do eiusmod\r\n"
				"tempor incididunt ut labore et dolore magna aliqua.\r\n"
				"OK\r\n"
				"Everything else should be omitted."
			};
	const std::string ok_test_string{
				"Lorem ipsum dolor sit amet,\r\n"
				"consectetur adipiscing elit, sed do eiusmod\r\n"
				"tempor incididunt ut labore et dolore magna aliqua.\r\n"
				"OK"
			};
	const std::string error_test_string{
				"Lorem ipsum dolor sit amet,\r\n"
				"consectetur adipiscing elit, sed do eiusmod\r\n"
				"tempor incididunt ut labore et dolore magna aliqua.\r\n"
				"ERROR\r\n"
			};
	EXPECT_TRUE(aura::parser::check_result(terminated_ok_test_string));
	EXPECT_TRUE(aura::parser::check_result(ok_test_string));
	EXPECT_FALSE(aura::parser::check_result(error_test_string));
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
	while (aura::parser::get_next_parameter(test_string, parameter)) {
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
	while (aura::parser::get_next_parameter(test_string, parameter)) {
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
	EXPECT_FALSE(aura::parser::get_next_parameter(test_string, parameter));
}
