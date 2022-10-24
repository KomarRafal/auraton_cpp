/*
 * next_device_id_ut.cpp
 */

#include <vector>
#include "next_device_id.hpp"
#include "gtest/gtest.h"

TEST(next_device_id_ut, next_device_id_one_token_ok)
{
	const std::string device_id_string{
		"ID: 20\r\n"
		"ADDRESS: 456223DA\r\n"
		"PCODE: 3005\r\n"
		"FVER: 1.5\r\n"
		"HVER: 1\r\n"
		"MANCODE: 30\r\n"
	};
	std::string_view device_id_string_view{device_id_string};
	aura::parser::next_device_id parser_ut;
	auto parse_result = parser_ut.parse(device_id_string_view);
	ASSERT_TRUE(static_cast<bool>(parse_result));
	EXPECT_EQ(*parse_result, device_id_string);
	EXPECT_TRUE(device_id_string_view.empty());
	parse_result = parser_ut.parse(device_id_string_view);
	EXPECT_FALSE(static_cast<bool>(parse_result));
}

TEST(next_device_id_ut, next_device_id_many_tokens_ok)
{
	const std::vector<std::string> devices{
		{
			"ID: 45\r\n"
			"Lorem ipsum dolor sit amet,\r\n"
			"consectetur adipiscing elit, sed do eiusmod\r\n"
			"tempor incididunt ut labore et dolore magna aliqua.\r\n"
			"OK\r\n"
			"\n\r\n\r\n\r PADDING"
		},
		{
			"ID: 1\r\n"
			"ADDRESS: 90FC4F9B\r\n"
			"PCODE: 300d\r\n"
			"FVER: 1.1\r\n"
			"HVER: 7\r\n"
			"MANCODE: 30\r\n"
		},
		{
			"ID: 20\r\n"
			"ADDRESS: 456223DA\r\n"
			"PCODE: 3005\r\n"
			"FVER: 1.5\r\n"
			"HVER: 1\r\n"
			"MANCODE: 30\r\n"
		}
	};
	const std::string device_id_string{
		devices[0] +
		devices[1] +
		devices[2]
	};
	std::vector<std::string>::size_type device_found = 0;
	std::string_view device_id_string_view(device_id_string);
	aura::parser::next_device_id parser_ut;
	auto parse_result = parser_ut.parse(device_id_string_view);
	while (parse_result.has_value()) {
		EXPECT_EQ(*parse_result, devices[device_found]);
		parse_result = parser_ut.parse(device_id_string_view);
		device_found++;
	}
	EXPECT_EQ(device_found, devices.size());
}
