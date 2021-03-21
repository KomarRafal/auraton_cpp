/*
 * at_parser_ut.cpp
 */

#include "gtest/gtest.h"
#include "parser/at.hpp"

TEST(at_parser_ut, at_parse_test_simple_ok)
{
	const std::string test_payload {
		"Lorem ipsum dolor sit amet,\r\n"
		"consectetur adipiscing elit, sed do eiusmod"
		"tempor incididunt ut labore et dolore magna aliqua.\r\n"
	};
	const std::string test_string {
		"AT:START\r\n" +
		test_payload +
		"AT:STOP\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::at parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_TRUE(parse_result);
	EXPECT_EQ(test_payload, static_cast<std::string>(test_string_view));
}

TEST(at_parser_ut, at_parse_test_empty)
{
	const std::string test_string {
		"AT:START\r\n"
		"AT:STOP\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::at parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_TRUE(parse_result);
	EXPECT_TRUE(test_string_view.empty());
}

TEST(at_parser_ut, at_parse_test_complex_ok)
{
	const std::string test_payload {
		"Lorem ipsum dolor sit amet,\r\n"
		"consectetur adipiscing elit, sed do eiusmod"
		"tempor incididunt ut labore et dolore magna aliqua.\r\n"
	};
	const std::string test_string {
		"Some prefixes"
		"AT:START\r\n" +
		test_payload +
		"AT:STOP\r\n"
		"Some postfixes"
	};
	std::string_view test_string_view(test_string);
	aura::parser::at parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_TRUE(parse_result);
	EXPECT_EQ(test_payload, static_cast<std::string>(test_string_view));
}

TEST(at_parser_ut, at_parse_double_begin)
{
	const std::string test_payload {
		"Lorem ipsum dolor sit amet,\r\n"
		"consectetur adipiscing elit, sed do eiusmod"
		"tempor incididunt ut labore et dolore magna aliqua.\r\n"
	};
	const std::string test_string {
		"AT:START\r\n"
		"AT:START\r\n" +
		test_payload +
		"AT:STOP\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::at parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_TRUE(parse_result);
	EXPECT_EQ(test_payload, static_cast<std::string>(test_string_view));
}

TEST(at_parser_ut, at_parse_triple_begin)
{
	const std::string test_payload {
		"Lorem ipsum dolor sit amet,\r\n"
		"consectetur adipiscing elit, sed do eiusmod"
		"tempor incididunt ut labore et dolore magna aliqua.\r\n"
	};
	const std::string test_string {
		"AT:START\r\n"
		"AT:START\r\n"
		"AT:START\r\n" +
		test_payload +
		"AT:STOP\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::at parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_TRUE(parse_result);
	EXPECT_EQ(test_payload, static_cast<std::string>(test_string_view));
}

TEST(at_parser_ut, at_parse_double_stop)
{
	const std::string test_payload {
		"Lorem ipsum dolor sit amet,\r\n"
		"consectetur adipiscing elit, sed do eiusmod"
		"tempor incididunt ut labore et dolore magna aliqua.\r\n"
	};
	const std::string test_string {
		"AT:START\r\n" +
		test_payload +
		"AT:STOP\r\n"
		"AT:STOP\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::at parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_TRUE(parse_result);
	EXPECT_EQ(test_payload, static_cast<std::string>(test_string_view));
}

TEST(at_parser_ut, at_parse_double_message)
{
	const std::string test_payload {
		"Lorem ipsum dolor sit amet,\r\n"
		"consectetur adipiscing elit, sed do eiusmod"
		"tempor incididunt ut labore et dolore magna aliqua.\r\n"
	};
	const std::string test_string {
		"AT:START\r\n" +
		test_payload +
		"AT:STOP\r\n"
		"AT:START\r\n"
		"Some other message"
		"AT:STOP\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::at parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_TRUE(parse_result);
	EXPECT_EQ(test_payload, static_cast<std::string>(test_string_view));
}

TEST(at_parser_ut, at_parse_begin_missing)
{
	const std::string test_payload {
		"Lorem ipsum dolor sit amet,\r\n"
		"consectetur adipiscing elit, sed do eiusmod"
		"tempor incididunt ut labore et dolore magna aliqua.\r\n"
	};
	const std::string test_string {
		test_payload +
		"AT:STOP\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::at parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_FALSE(parse_result);
}

TEST(at_parser_ut, at_parse_end_missing)
{
	const std::string test_payload {
		"Lorem ipsum dolor sit amet,\r\n"
		"consectetur adipiscing elit, sed do eiusmod"
		"tempor incididunt ut labore et dolore magna aliqua.\r\n"
	};
	const std::string test_string {
		"AT:START\r\n" +
		test_payload
	};
	std::string_view test_string_view(test_string);
	aura::parser::at parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_FALSE(parse_result);
}

TEST(at_parser_ut, at_parse_end_before_begin)
{
	const std::string test_payload {
		"Lorem ipsum dolor sit amet,\r\n"
		"consectetur adipiscing elit, sed do eiusmod"
		"tempor incididunt ut labore et dolore magna aliqua.\r\n"
	};
	const std::string test_string {
		"AT:STOP\r\n" +
		test_payload +
		"AT:START\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::at parser_ut;
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_FALSE(parse_result);
}

TEST(at_parser_ut, at_parse_complex)
{
	const std::string test_payload_1 {
		"Lorem ipsum dolor sit amet,\r\n"
		"consectetur adipiscing elit,"
	};
	const std::string test_payload_2 {
		"sed do eiusmod"
		"tempor incididunt ut labore et dolore magna aliqua.\r\n"
	};
	const std::string test_string {
		"AT:START\r\n" +
		test_payload_1 +
		"AT:STOP\r\n"
		"AT:START\r\n" +
		test_payload_2 +
		"AT:STOP\r\n"
	};

	aura::parser::at parser_ut;
	std::string_view test_string_view_1(test_string);
	{
		const auto parse_result = parser_ut.parse(test_string_view_1);
		EXPECT_TRUE(parse_result);
		EXPECT_EQ(test_payload_1, static_cast<std::string>(test_string_view_1));
	}
	std::string_view test_string_view(test_string.c_str() + test_string_view_1.size() + strlen("AT:START\r\n") + strlen("AT:STOP\r\n"));
	{
		const auto parse_result = parser_ut.parse(test_string_view);
		EXPECT_TRUE(parse_result);
		EXPECT_EQ(test_payload_2, static_cast<std::string>(test_string_view));
	}
}
