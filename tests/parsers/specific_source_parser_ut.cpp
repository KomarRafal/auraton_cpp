/*
 * specific_source_parser_ut.cpp
 */

#include "gtest/gtest.h"
#include "parsers/specific_source.hpp"

const std::string test_payload {
	"Lorem ipsum dolor sit amet,\r\n"
	"consectetur adipiscing elit, sed do eiusmod"
	"tempor incididunt ut labore et dolore magna aliqua.\r\n"
};

TEST(specific_source_parser_ut, specific_source_parse_test_ok)
{
	const std::string specific_source {
		"SPECIFIC_SOURCE"
	};
	const std::string test_string {
		"SOURCE:" +
		specific_source +
		"\r\n" +
		test_payload
	};
	std::string_view test_string_view(test_string);
	aura::parser::specific_source parser_ut(specific_source);
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_TRUE(parse_result);
	EXPECT_EQ(test_payload, static_cast<std::string>(test_string_view));
}

TEST(specific_source_parser_ut, specific_source_parse_test_wrong_token)
{
	const std::string specific_source {
		"SPECIFIC_SOURCE"
	};
	const std::string wrong_source {
		"WRONG_SOURCE"
	};
	const std::string test_string {
		"SOOURCE:" +
		wrong_source +
		"\r\n" +
		test_payload
	};
	std::string_view test_string_view(test_string);
	aura::parser::specific_source parser_ut(specific_source);
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_FALSE(parse_result);
}
