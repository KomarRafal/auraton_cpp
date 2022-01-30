/*
 * at_traverse_ut.cpp
 */

#include "at_traverse.hpp"
#include "gtest/gtest.h"

TEST(at_traverse_ut, get_next_block_ok)
{
	const std::string test_block_start {
		"AT:START\r\n"
	};

	const std::string test_block_content {
		"Lorem ipsum dolor sit amet,\r\n"
	};

	const std::string test_block_end {
		"AT:STOP\r\n"
	};
	const std::string noise {
		"consectetur adipiscing elit,"
	};

	const uint32_t test_block_count = 32;
	std::string test_string{ noise };
	for (uint32_t idx = 0; idx < test_block_count; idx++) {
		test_string.append(test_block_start);
		test_string.append(test_block_content);
		test_string.append(std::to_string(idx));
		test_string.append(test_block_end);
		test_string.append(noise);
	}

	std::string_view test_string_view{ test_string };
	aura::parser::at_traverse traverse_uut{ test_string_view };
	for (uint32_t idx = 0; idx < test_block_count; idx++) {
		const auto block = traverse_uut.get_next_block();
		const std::string expected_block = {
			test_block_start +
			test_block_content +
			std::to_string(idx) +
			test_block_end
		};
		EXPECT_TRUE(static_cast<bool>(block));
		EXPECT_EQ(block.value_or(""), expected_block);
	}
	const auto block = traverse_uut.get_next_block();
	EXPECT_FALSE(static_cast<bool>(block));
}

TEST(at_traverse_ut, block_inside_block)
{
	const std::string test_block {
		"AT:START\r\n"
		"consectetur adipiscing elit, sed do eiusmod"
		"AT:STOP\r\n"
	};
	const std::string test_string {
		"AT:START\r\n"
		"Lorem ipsum dolor sit amet,\r\n" +
		test_block +
		"tempor incididunt ut labore et dolore magna aliqua.\r\n"
		"AT:STOP\r\n"
	};

	std::string_view test_string_view{ test_string };
	aura::parser::at_traverse traverse_uut{ test_string_view };
	auto block = traverse_uut.get_next_block();
	EXPECT_TRUE(static_cast<bool>(block));
	EXPECT_EQ(block.value_or(""), test_block);
	block = traverse_uut.get_next_block();
	EXPECT_FALSE(static_cast<bool>(block));
}

TEST(at_traverse_ut, test_remaining_string_view)
{
	const std::string test_block_1 {
		"AT:START\r\n"
		"Lorem ipsum dolor sit amet,\r\n"
		"AT:STOP\r\n"
	};
	const std::string test_block_2 {
		"AT:START\r\n"
		"consectetur adipiscing elit, sed do eiusmod"
		"AT:STOP\r\n"
	};
	const std::string test_noise {
		"tempor incididunt ut labore et dolore magna aliqua.\r\n"
	};

	const std::string test_string = {
			test_block_1 +
			test_block_2 +
			test_noise
	};

	std::string_view test_string_view{ test_string };
	aura::parser::at_traverse traverse_uut{ test_string_view };
	auto block = traverse_uut.get_next_block();
	EXPECT_TRUE(static_cast<bool>(block));
	EXPECT_EQ(block.value_or(""), test_block_1);
	EXPECT_EQ(static_cast<std::string>(test_string_view), test_block_2 + test_noise);
	block = traverse_uut.get_next_block();
	EXPECT_TRUE(static_cast<bool>(block));
	EXPECT_EQ(block.value_or(""), test_block_2);
	EXPECT_EQ(static_cast<std::string>(test_string_view), test_noise);
	block = traverse_uut.get_next_block();
	EXPECT_FALSE(static_cast<bool>(block));
	EXPECT_EQ(static_cast<std::string>(test_string_view), test_noise);
}
