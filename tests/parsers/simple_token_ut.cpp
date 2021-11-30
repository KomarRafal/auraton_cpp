/*
 * simple_token_ut.cpp
 */

#include "gtest/gtest.h"
#include "simple_token.hpp"

class simple_token_test :
	public aura::parser::simple_token {
public:
	simple_token_test(std::string test_token, std::string test_value) :
		token(test_token),
		value(test_value) {
	}

	virtual const std::string& get_token() const {
		return token;
	}

	virtual bool is_value_ok(const std::string_view& test_value) const {
		return (static_cast<std::string>(test_value) == value);
	}

private:
	std::string token;
	std::string value;
};

TEST(simple_token_ut, simple_token_test_ok)
{
	const std::string test_value {
		"some value"
	};

	const std::string test_token {
		"some_token:"
	};

	const std::string test_string {
		test_token +
		test_value +
		"\r\n"
	};
	std::string_view test_string_view(test_string);
	simple_token_test parser_ut(test_token, test_value);
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_TRUE(parse_result.has_value());
	EXPECT_EQ(*parse_result, test_value);
	EXPECT_TRUE(test_string_view.empty());
}

TEST(simple_token_ut, simple_token_test_fail_token)
{
	const std::string test_value {
		"some value"
	};

	const std::string test_token {
		"some_token:"
	};

	const std::string fail_token {
		"sometoken:"
	};

	const std::string test_string {
		fail_token +
		test_value +
		"\r\n"
	};
	std::string_view test_string_view(test_string);
	simple_token_test parser_ut(test_token, test_value);
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_FALSE(parse_result.has_value());
}

TEST(simple_token_ut, simple_token_test_fail_value)
{
	const std::string test_value {
		"some value"
	};

	const std::string test_token {
		"some_token:"
	};

	const std::string fail_value {
		"wrong value"
	};

	const std::string test_string {
		test_token +
		fail_value +
		"\r\n"
	};
	std::string_view test_string_view(test_string);
	simple_token_test parser_ut(test_token, test_value);
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_FALSE(parse_result.has_value());
	EXPECT_TRUE(test_string_view.empty());
}

TEST(simple_token_ut, simple_token_test_multiline_ok)
{
	const std::string test_value {
		"some value"
	};

	const std::string test_token {
		"some_token:"
	};

	const std::string multiline {
		"Lorem ipsum dolor sit amet,\r\n"
		"consectetur adipiscing elit, sed do eiusmod"
		"tempor incididunt ut labore et dolore magna aliqua.\r\n"
	};

	const std::string test_string {
		test_token +
		test_value +
		"\r\n" +
		multiline

	};
	std::string_view test_string_view(test_string);
	simple_token_test parser_ut(test_token, test_value);
	const auto parse_result = parser_ut.parse(test_string_view);
	EXPECT_TRUE(parse_result.has_value());
	EXPECT_EQ(*parse_result, test_value);
	EXPECT_FALSE(test_string_view.empty());
	EXPECT_EQ(static_cast<std::string>(test_string_view), multiline);
}
