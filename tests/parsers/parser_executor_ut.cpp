/*
 * parser_executor_ut.cpp
 */

#include "parsers/commands/specific_command.hpp"
#include "parsers/specific_source.hpp"
#include "parsers/parser_executor.hpp"
#include "parsers/parser_if.hpp"
#include "parsers/status.hpp"
#include "parsers/at.hpp"
#include "gtest/gtest.h"

class parser_executor_ut : public ::testing::Test
{
protected:
	virtual void SetUp() override {
		aura::parser::parser_if::parser_ptr at_parser = std::make_unique<aura::parser::at>();
		aura::parser::parser_if::parser_ptr source_parser = std::make_unique<aura::parser::specific_source>("TEST_SOURCE");
		aura::parser::parser_if::parser_ptr command_parser = std::make_unique<aura::parser::commands::specific_command>("TEST_COMMAND");
		aura::parser::parser_if::parser_ptr status_parser = std::make_unique<aura::parser::status>();

		parse_algorithm.push_back(std::move(at_parser));
		parse_algorithm.push_back(std::move(source_parser));
		parse_algorithm.push_back(std::move(command_parser));
		parse_algorithm.push_back(std::move(status_parser));
	}
	aura::parser::parser_if::parser_algorithm_t parse_algorithm;

	const std::string test_payload {
		"Lorem ipsum dolor sit amet,\r\n"
		"consectetur adipiscing elit, sed do eiusmod"
		"tempor incididunt ut labore et dolore magna aliqua.\r\n"
	};
};

TEST_F(parser_executor_ut, parser_executor_test_simpl_ok)
{
	const std::string test_string {
		"AT:START\r\n"
		"SOURCE:TEST_SOURCE\r\n"
		"COMMAND:TEST_COMMAND\r\n"
		"STATUS:OK\r\n" +
		test_payload +
		"AT:STOP\r\n"
	};
	std::string_view test_string_view(test_string);
	const bool parse_result = aura::parser::parser_executor::execute(test_string_view, parse_algorithm);
	EXPECT_TRUE(parse_result);
	EXPECT_EQ(test_payload, static_cast<std::string>(test_string_view));
}

TEST_F(parser_executor_ut, parser_executor_test_fail)
{
	const std::string test_payload {
		"Lorem ipsum dolor sit amet,\r\n"
		"consectetur adipiscing elit, sed do eiusmod"
		"tempor incididunt ut labore et dolore magna aliqua.\r\n"
	};

	const std::string test_string {
		"AT:START\r\n"
		"SOURCE:WRONG_SOURCE\r\n"
		"COMMAND:TEST_COMMAND\r\n"
		"STATUS:OK\r\n" +
		test_payload +
		"AT:STOP\r\n"
	};
	std::string_view test_string_view(test_string);
	const bool parse_result = aura::parser::parser_executor::execute(test_string_view, parse_algorithm);
	EXPECT_FALSE(parse_result);
}

TEST_F(parser_executor_ut, parser_executor_algorithm_fail_1)
{
	const std::string test_payload {
		"Lorem ipsum dolor sit amet,\r\n"
		"consectetur adipiscing elit, sed do eiusmod"
		"tempor incididunt ut labore et dolore magna aliqua.\r\n"
	};

	const std::string test_string {
		"AT:START\r\n"
		"SOURCE:WRONG_SOURCE\r\n"
		"SOME_LAYER:OK\r\n"
		"COMMAND:TEST_COMMAND\r\n"
		"STATUS:OK\r\n" +
		test_payload +
		"AT:STOP\r\n"
	};
	std::string_view test_string_view(test_string);
	const bool parse_result = aura::parser::parser_executor::execute(test_string_view, parse_algorithm);
	EXPECT_FALSE(parse_result);
}

TEST_F(parser_executor_ut, parser_executor_algorithm_fail_2)
{
	const std::string test_payload {
		"Lorem ipsum dolor sit amet,\r\n"
		"consectetur adipiscing elit, sed do eiusmod"
		"tempor incididunt ut labore et dolore magna aliqua.\r\n"
	};

	const std::string test_string {
		"AT:START\r\n"
		"COMMAND:TEST_COMMAND\r\n"
		"STATUS:OK\r\n" +
		test_payload +
		"AT:STOP\r\n"
	};
	std::string_view test_string_view(test_string);
	const bool parse_result = aura::parser::parser_executor::execute(test_string_view, parse_algorithm);
	EXPECT_FALSE(parse_result);
}

