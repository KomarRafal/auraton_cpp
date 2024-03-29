/*
 * connection_ut.cpp
 */
#include <cstring>
#include <memory>

#include "connection.hpp"
#include "gtest/gtest.h"
#include "timeout.hpp"

TEST(connection_ut, open_ok)
{
	const std::string device_port{"COM6"};
	const uint32_t baud_rate{115200};
	aura::connection connection_uut{device_port};
	auto& serial_dev = connection_uut.get_serial_dev();

	EXPECT_CALL(serial_dev, openDevice(testing::_, baud_rate))
    	.WillOnce(testing::Return(1));
	EXPECT_TRUE(connection_uut.open());
}

TEST(connection_ut, open_fail)
{
	const std::string device_port{"COM6"};
	const uint32_t baud_rate{115200};
	aura::connection connection_uut{device_port};
	auto& serial_dev = connection_uut.get_serial_dev();

	EXPECT_CALL(serial_dev, openDevice(testing::_, baud_rate))
    	.WillOnce(testing::Return(0));
	EXPECT_FALSE(connection_uut.open());
}

TEST(connection_ut, close)
{
	const std::string device_port{"COM6"};
	aura::connection connection_uut{device_port};
	auto& serial_dev = connection_uut.get_serial_dev();

	EXPECT_CALL(serial_dev, closeDevice())
		.WillOnce(testing::Return());

	connection_uut.close();
}

TEST(connection_ut, wait_for_read_true)
{
	const uint32_t max_time_ms{550};
	const std::string device_port{"COM6"};
	aura::connection connection_uut{device_port};
	auto& serial_dev = connection_uut.get_serial_dev();
	TimeoutRAII timeout_raii;
	auto& timeout_instance = timeout_raii.get_instance();

	EXPECT_CALL(serial_dev, available())
		.WillOnce(testing::Return(0))
		.WillOnce(testing::Return(0))
		.WillOnce(testing::Return(0))
		.WillOnce(testing::Return(10));

	EXPECT_CALL(timeout_instance, sleep_for_ms(100))
		.Times(3);

	EXPECT_TRUE(connection_uut.wait_for_read(max_time_ms));
}

TEST(connection_ut, wait_for_read_false)
{
	const uint32_t max_time_ms{550};
	const std::string device_port{"COM6"};
	aura::connection connection_uut{device_port};
	auto& serial_dev = connection_uut.get_serial_dev();
	TimeoutRAII timeout_raii;
	auto& timeout_instance = timeout_raii.get_instance();

	EXPECT_CALL(serial_dev, available())
		.WillRepeatedly(testing::Return(0));

	EXPECT_CALL(timeout_instance, sleep_for_ms(100))
		.Times(5);

	EXPECT_CALL(timeout_instance, sleep_for_ms(50))
		.Times(1);

	EXPECT_FALSE(connection_uut.wait_for_read(max_time_ms));
}

ACTION_TEMPLATE(SetArgNPointeeTo, HAS_1_TEMPLATE_PARAMS(unsigned, uIndex), AND_2_VALUE_PARAMS(pData, uiDataSize))
{
    std::memcpy(std::get<uIndex>(args), pData, uiDataSize);
}

TEST(connection_ut, send_command_ok)
{
	const std::string device_port{"COM6"};
	aura::connection connection_uut{device_port};
	auto& serial_dev = connection_uut.get_serial_dev();
	const std::string command{"COMMAND_UUT"};
	const char receive_buffer[] = "Lorem ipsum";
	const size_t buffer_size = sizeof(receive_buffer);
	auto& timeout_instance = Timeout::get_instance();

	EXPECT_CALL(timeout_instance, sleep_for_ms(testing::_))
		.Times(0);

	EXPECT_CALL(serial_dev, flushReceiver())
		.Times(1);

	EXPECT_CALL(serial_dev, writeBytes(command.c_str(), command.length()))
		.Times(1);

	EXPECT_CALL(serial_dev, readBytes(testing::_, buffer_size, testing::_, testing::_))
		.WillOnce(testing::DoAll(
				SetArgNPointeeTo<0>(std::begin(receive_buffer), buffer_size),
				testing::Return(buffer_size))
				);

	auto response = connection_uut.send_command(command, buffer_size, 0);
	EXPECT_EQ(response, std::string(receive_buffer));
}

TEST(connection_ut, send_command_ok_timeout)
{
	const std::string device_port{"COM6"};
	aura::connection connection_uut{device_port};
	auto& serial_dev = connection_uut.get_serial_dev();
	const std::string command{"COMMAND_UUT"};
	const char receive_buffer[] = "Lorem ipsum";
	const size_t buffer_size = sizeof(receive_buffer);
	const uint32_t timeout = 125;
	TimeoutRAII timeout_raii;
	auto& timeout_instance = timeout_raii.get_instance();

	EXPECT_CALL(timeout_instance, sleep_for_ms(timeout))
		.Times(1);

	EXPECT_CALL(serial_dev, flushReceiver())
		.Times(1);

	EXPECT_CALL(serial_dev, writeBytes(command.c_str(), command.length()))
		.Times(1);

	EXPECT_CALL(serial_dev, readBytes(testing::_, buffer_size, testing::_, testing::_))
		.WillOnce(testing::DoAll(
				SetArgNPointeeTo<0>(std::begin(receive_buffer), buffer_size),
				testing::Return(buffer_size))
				);

	auto response = connection_uut.send_command(command, buffer_size, timeout);
	EXPECT_EQ(response, std::string(receive_buffer));
}

TEST(connection_ut, send_command_fail)
{
	const std::string device_port{"COM6"};
	aura::connection connection_uut{device_port};
	auto& serial_dev = connection_uut.get_serial_dev();
	const std::string command{"COMMAND_UT"};
	const uint8_t max_buffer_length{14};
	TimeoutRAII timeout_raii;
	auto& timeout_instance = timeout_raii.get_instance();

	EXPECT_CALL(timeout_instance, sleep_for_ms(testing::_))
		.Times(0);

	EXPECT_CALL(serial_dev, flushReceiver())
		.Times(1);

	EXPECT_CALL(serial_dev, writeBytes(command.c_str(), command.length()))
		.Times(1);

	EXPECT_CALL(serial_dev, readBytes(testing::_, max_buffer_length, testing::_, testing::_))
		.WillOnce(testing::Return(0));

	auto response = connection_uut.send_command(command, max_buffer_length, 0);
	EXPECT_TRUE(response.empty());
}

TEST(connection_ut, read_event_ok)
{
	const std::string device_port{"COM6"};
	aura::connection connection_uut{device_port};
	auto& serial_dev = connection_uut.get_serial_dev();
	const char receive_buffer[] = "XXX\r\nEVENT_UT\r\nOK\r\nX";
	const size_t buffer_size = sizeof(receive_buffer);

	EXPECT_CALL(serial_dev, available())
		.WillRepeatedly(testing::Return(buffer_size));

	EXPECT_CALL(serial_dev, readBytes(testing::_, testing::_, testing::_, testing::_))
		.WillOnce(testing::DoAll(
				SetArgNPointeeTo<0>(std::begin(receive_buffer), buffer_size),
				testing::Return(buffer_size))
				);

	const auto event = connection_uut.read_event(buffer_size, 100);
	EXPECT_EQ(event, receive_buffer);
}

TEST(connection_ut, check_event_fail_empty_buffer)
{
	const std::string device_port{"COM6"};
	aura::connection connection_uut{device_port};
	auto& serial_dev = connection_uut.get_serial_dev();
	const std::string test_event{"EVENT_UT"};

	EXPECT_CALL(serial_dev, readBytes(testing::_, testing::_, testing::_, testing::_))
		.WillOnce(testing::Return(0));

	auto const event = connection_uut.read_event(10, 0);
	EXPECT_TRUE(event.empty());
}
