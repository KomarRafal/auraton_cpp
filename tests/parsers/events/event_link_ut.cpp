/*
 * event_link_ut.cpp
 */

//#include "events/specific_event.hpp"
#include "parser_executor.hpp"
#include "events/link.hpp"
#include "gtest/gtest.h"

TEST(event_link_ut, event_link_ok)
{
	const std::string event_string {
		"DIRECTION:RECEIVE\r\n"
		"ID: 1\r\n"
		"ADDRESS: 30090005\r\n"
		"PCODE: 3009\r\n"
		"FVER: 1.11\r\n"
		"HVER: 1.0\r\n"
		"MANCODE: 30\r\n"
		"CODE: 32\r\n"
		"CHANNEL: 0\r\n"
		"FLAG OWN: 0\r\n"
		"FLAG WRITEABLE: 1\r\n"
		"VALUE: 0\r\n"
	};
	std::string_view event_string_view(event_string);
	aura::parser::events::link parser_ut;
	const auto parse_result = parser_ut.parse(event_string_view);
	EXPECT_TRUE(parse_result);
}

TEST(event_link_ut, event_link_wrong_direction)
{
	const std::string event_string {
		"DIRECTION:TRANSMIT\r\n"
	};
	std::string_view event_string_view(event_string);
	aura::parser::events::link parser_ut;
	const auto parse_result = parser_ut.parse(event_string_view);
	EXPECT_FALSE(parse_result);
}

TEST(event_link_ut, event_link_wrong_token)
{
	const std::string event_string {
		" DIRECTION:TRANSMIT\r\n"
	};
	std::string_view event_string_view(event_string);
	aura::parser::events::link parser_ut;
	const auto parse_result = parser_ut.parse(event_string_view);
	EXPECT_FALSE(parse_result);
}

TEST(event_link_ut, event_link_missing_token)
{
	const std::string event_string {
		"ID: 1\r\n"
		"ADDRESS: 30090005\r\n"
		"PCODE: 3009\r\n"
		"FVER: 1.11\r\n"
		"HVER: 1.0\r\n"
		"MANCODE: 30\r\n"
		"CODE: 32\r\n"
		"CHANNEL: 0\r\n"
		"FLAG OWN: 0\r\n"
		"FLAG WRITEABLE: 1\r\n"
		"VALUE: 0\r\n"
	};
	std::string_view event_string_view(event_string);
	aura::parser::events::link parser_ut;
	const auto parse_result = parser_ut.parse(event_string_view);
	EXPECT_FALSE(parse_result);
}

TEST(event_link_ut, event_link_algorithm_ok)
{
	const std::string event_device {
		"ID: 1\r\n"
		"ADDRESS: 30090005\r\n"
		"PCODE: 3009\r\n"
		"FVER: 1.11\r\n"
		"HVER: 1.0\r\n"
		"MANCODE: 30\r\n"
		"CODE: 32\r\n"
		"CHANNEL: 0\r\n"
		"FLAG OWN: 0\r\n"
		"FLAG WRITEABLE: 1\r\n"
		"VALUE: 0\r\n"
	};
	const std::string event_string {
		"AT:START\r\n"
		"SOURCE:EVENT\r\n"
		"EVENT:LINK\r\n"
		"STATUS:OK\r\n"
		"DIRECTION:RECEIVE\r\n" +
		event_device +
		"AT:STOP\r\n"
	};
	std::string_view event_string_view(event_string);
	const auto status = aura::parser::parser_executor::execute(
			event_string_view,
			aura::parser::events::link_builder::build());
	EXPECT_TRUE(static_cast<bool>(status));
	EXPECT_EQ(std::string{event_string_view}, event_device);
}

