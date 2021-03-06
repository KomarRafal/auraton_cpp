/*
 * device_ut.cpp
 */
#include <functional>
#include <utility>
#include <string>

#include "gtest/gtest.h"
#include "device.hpp"
#include "parser.hpp"

class device_ut : public ::testing::Test {

protected:
	const std::string device_1_address{"1234ABCD"};
	const int address = 305441741;
	const std::string device_1 {
		"Lorem ipsum dolor sit amet,"
		"HVER: 28\r\n"
		"MANCODE: 2020\r\n"
		"consectetur adipiscing elit, sed do eiusmod\r\n"
		"FVER: 1.5.0\r\n"
		"PCODE: 23651.23\r\n"
		"tempor incididunt ut labore et dolore magna aliqua.\r\n"
		"ADDRESS: " + device_1_address};

    bool is_empty(aura::device& dev_uut) {
    	const bool all_fields_empty = dev_uut.get_product_code().empty() &&
    			dev_uut.get_fw_version().empty() &&
				dev_uut.get_hw_version().empty() &&
				dev_uut.get_manufacture_code().empty() &&
				(dev_uut.get_address() == 0);
        return all_fields_empty;
    }

};

TEST_F(device_ut, empty_ctor)
{
	aura::device dev_uut{};
	EXPECT_TRUE(is_empty(dev_uut));
}

TEST_F(device_ut, ctor)
{
	aura::device dev_uut{device_1};
	EXPECT_FALSE(is_empty(dev_uut));
	EXPECT_EQ(dev_uut.get_product_code(),
			aura::parser::parse(device_1, aura::device::PRODUCT_CODE_TOKEN));
	EXPECT_EQ(dev_uut.get_fw_version(),
			aura::parser::parse(device_1, aura::device::FW_VERSION_TOKEN));
	EXPECT_EQ(dev_uut.get_hw_version(),
			aura::parser::parse(device_1, aura::device::HW_VERSION_TOKEN));
	EXPECT_EQ(dev_uut.get_manufacture_code(),
			aura::parser::parse(device_1, aura::device::MANUFACTURE_CODE_TOKEN));
	EXPECT_EQ(dev_uut.get_address(), address);
}

TEST_F(device_ut, clear)
{
	aura::device dev_uut{device_1};
	dev_uut.clear();
	EXPECT_TRUE(is_empty(dev_uut));
}

TEST_F(device_ut, simple_set_get_test)
{
	using setter_t = std::function<void(const std::string&)>;
	using getter_t = std::function<std::string(void)>;
	aura::device dev_uut{device_1};

	std::vector<std::tuple<const std::string, setter_t, getter_t>> set_get {
		{ "Lorem ipsum dolor",
			[&](const std::string& value){ dev_uut.set_product_code(value); },
			[&]()->std::string{ return dev_uut.get_product_code(); }
		},
		{ "sit amet,\r\n",
			[&](const std::string& value){ dev_uut.set_fw_version(value); },
			[&]()->std::string{ return dev_uut.get_fw_version(); }
		},
		{ "consectetur adipiscing",
			[&](const std::string& value){ dev_uut.set_hw_version(value); },
			[&]()->std::string{ return dev_uut.get_hw_version(); }
		},
		{ "elit",
			[&](const std::string& value){ dev_uut.set_manufacture_code(value); },
			[&]()->std::string{ return dev_uut.get_manufacture_code(); }
		}
	};

	for (auto set_get_tuple : set_get) {
		const auto [ test_value, set, get ] = set_get_tuple;
		set(test_value);
		EXPECT_EQ(test_value, get());
	}
}

TEST_F(device_ut, set_get_address_test)
{
	aura::device dev_uut{device_1};

	dev_uut.set_address("0xAB56EECD");
	EXPECT_EQ(0xAB56EECD, dev_uut.get_address());
	dev_uut.set_address("0");
	EXPECT_EQ(0, dev_uut.get_address());
	dev_uut.set_address("78564");
	EXPECT_EQ(0x78564, dev_uut.get_address());
	dev_uut.set_address("");
	EXPECT_EQ(0, dev_uut.get_address());
	dev_uut.set_address(0x345BCE);
	EXPECT_EQ(0x345BCE, dev_uut.get_address());
}
