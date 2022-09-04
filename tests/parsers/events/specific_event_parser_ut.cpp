/*
 * specific_event_parser_ut.cpp
 */

#include "gtest/gtest.h"
#include "events/specific_event.hpp"

const std::string test_payload {
	"Lorem ipsum dolor sit amet,\r\n"
	"consectetur adipiscing elit, sed do eiusmod"
	"tempor incididunt ut labore et dolore magna aliqua.\r\n"
};

TEST(specific_event_parser_ut, specific_event_test_ok)
{
	const std::string event {
		"SPECIFIC_EVENT"
	};
	const std::string test_string {
		"EVENT:" +
		event +
		"\r\n" +
		test_payload
	};
	std::string_view test_string_view(test_string);
	aura::parser::events::specific_event parser_ut(event);
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_TRUE(parse_result);
	EXPECT_EQ(test_payload, static_cast<std::string>(test_string_view));
}

TEST(specific_event_parser_ut, specific_event_test_fail)
{
	const std::string event {
		"SPECIFIC_EVENT"
	};
	const std::string wrong_event {
		"WRONG_EVENT"
	};
	const std::string test_string {
		"EVENT:" +
		event +
		"\r\n"
	};
	std::string_view test_string_view(test_string);
	aura::parser::events::specific_event parser_ut(wrong_event);
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_FALSE(parse_result);
}
