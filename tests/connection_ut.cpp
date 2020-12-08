/*
 * connection_ut.cpp
 */
#include <memory>

#include "connection.hpp"
#include "gtest/gtest.h"
#include "timeout.hpp"

TEST(connection_ut, open_ok)
{
	const std::string device_port = "COM6";
	const uint32_t baud_rate = 115200;
	aura::connection connection_uut{device_port};
	auto& serial_dev = connection_uut.get_serial_dev();

	EXPECT_CALL(serial_dev, openDevice(testing::_, baud_rate))
    	.WillOnce(testing::Return(1));
	EXPECT_TRUE(connection_uut.open());
}

TEST(connection_ut, open_fail)
{
	const std::string device_port = "COM6";
	const uint32_t baud_rate = 115200;
	aura::connection connection_uut{device_port};
	auto& serial_dev = connection_uut.get_serial_dev();

	EXPECT_CALL(serial_dev, openDevice(testing::_, baud_rate))
    	.WillOnce(testing::Return(0));
	EXPECT_FALSE(connection_uut.open());
}

TEST(connection_ut, close)
{
	const std::string device_port = "COM6";
	aura::connection connection_uut{device_port};
	auto& serial_dev = connection_uut.get_serial_dev();

	EXPECT_CALL(serial_dev, closeDevice())
		.WillOnce(testing::Return());

	connection_uut.close();
}

TEST(connection_ut, wait_for_read_true)
{
	const uint32_t max_time_ms = 550;
	const std::string device_port = "COM6";
	aura::connection connection_uut{device_port};
	auto& serial_dev = connection_uut.get_serial_dev();
	auto* timeout_instance = Timeout::get_instance();

	EXPECT_CALL(serial_dev, available())
		.WillOnce(testing::Return(0))
		.WillOnce(testing::Return(0))
		.WillOnce(testing::Return(0))
		.WillOnce(testing::Return(10));

	EXPECT_CALL(*timeout_instance, sleep_for_ms(100))
		.Times(3);

	EXPECT_TRUE(connection_uut.wait_for_read(max_time_ms));
}

TEST(connection_ut, wait_for_read_false)
{
	const uint32_t max_time_ms = 550;
	const std::string device_port = "COM6";
	aura::connection connection_uut{device_port};
	auto& serial_dev = connection_uut.get_serial_dev();
	auto* timeout_instance = Timeout::get_instance();

	EXPECT_CALL(serial_dev, available())
		.WillRepeatedly(testing::Return(0));

	EXPECT_CALL(*timeout_instance, sleep_for_ms(100))
		.Times(5);

	EXPECT_CALL(*timeout_instance, sleep_for_ms(50))
		.Times(1);

	EXPECT_FALSE(connection_uut.wait_for_read(max_time_ms));
}
