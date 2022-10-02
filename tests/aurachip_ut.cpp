/*
 * aurachip_ut.cpp
 */
#include <cstring>

#include "parameter.hpp"
#include "aurachip.hpp"
#include "timeout.hpp"
#include "gtest/gtest.h"

ACTION_TEMPLATE(SetArgNPointeeTo, HAS_1_TEMPLATE_PARAMS(unsigned, uIndex), AND_1_VALUE_PARAMS(response_string))
{
    std::strcpy(static_cast<char*>(std::get<uIndex>(args)), response_string.c_str());
}

MATCHER_P(StrEqVoidPointer, expected, "") {
	return std::string {static_cast<const char* const>(arg)} == expected;
}

TEST(aurachip_ut, initialize)
{
	const std::string device_port{"COM6"};
	aura::chip aura_chip_uut{device_port};
	auto& serial_dev = aura_chip_uut.get_connection().get_serial_dev();
	TimeoutRAII timeout_raii;
	auto& timeout_instance = timeout_raii.get_instance();

	const std::string version_cmd {"AT+VER?\n"};
	const std::string address_cmd {"AT+ADDR?\n"};

	const std::string device_version {
		"AT:START\r\n"
		"SOURCE:COMMAND\r\n"
		//TODO: remove space after fix
		"COMMAND: VER?\r\n"
		"STATUS:OK\r\n"
		"HVER: 28\r\n"
		"MANCODE: 2020\r\n"
		"FVER: 1.5.0\r\n"
		"PCODE: 23651.23\r\n"
		"AT:STOP\r\n"
		};

	const std::string device_address {
		"AT:START\r\n"
		"SOURCE:COMMAND\r\n"
		//TODO: remove space after fix
		"COMMAND: ADDR?\r\n"
		"STATUS:OK\r\n"
		"ADDRESS: 3054412\r\n"
		"AT:STOP\r\n"
		};

	const aura::device model_device {device_version + device_address};

	EXPECT_CALL(timeout_instance, sleep_for_ms(testing::_))
		.Times(2);

	EXPECT_CALL(serial_dev, flushReceiver())
		.Times(2);

	EXPECT_CALL(serial_dev, writeBytes(StrEqVoidPointer(version_cmd), testing::_))
		.Times(1);

	EXPECT_CALL(serial_dev, writeBytes(StrEqVoidPointer(address_cmd), testing::_))
		.Times(1);

	EXPECT_CALL(serial_dev, readBytes(testing::_, testing::_, testing::_, testing::_))
		.WillOnce(testing::DoAll(
				SetArgNPointeeTo<0>(device_version),
				testing::Return(device_version.length()))
				)
		.WillOnce(testing::DoAll(
				SetArgNPointeeTo<0>(device_address),
				testing::Return(device_address.length()))
				);

	EXPECT_FALSE(aura_chip_uut.is_initialize());
	aura_chip_uut.initialize();
	EXPECT_EQ(static_cast<aura::device>(aura_chip_uut), model_device);
	EXPECT_TRUE(aura_chip_uut.is_initialize());
}

TEST(aurachip_ut, link_ok)
{
	const std::string device_port{"COM6"};
	aura::chip aura_chip_uut{device_port};
	auto& serial_dev = aura_chip_uut.get_connection().get_serial_dev();
	TimeoutRAII timeout_raii;
	auto& timeout_instance = timeout_raii.get_instance();
	const std::string correct_answer{
		"AT:START\r\n"
		"SOURCE:COMMAND\r\n"
		"COMMAND: LINK\r\n"
		"STATUS:OK\r\n"
		"AT:STOP\r\n"
		};
	const std::string correct_event{
		"AT:START\r\n"
		"SOURCE:EVENT\r\n"
		"EVENT:LINK\r\n"
		"STATUS:OK\r\n"
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
		"AT:STOP\r\n"
	};

	EXPECT_CALL(timeout_instance, sleep_for_ms(testing::_))
		.Times(1);

	EXPECT_CALL(serial_dev, flushReceiver())
		.Times(1);

	EXPECT_CALL(serial_dev, writeBytes(StrEqVoidPointer("AT+LINK\n"), testing::_))
		.Times(1);

	EXPECT_CALL(serial_dev, readBytes(testing::_, testing::_, testing::_, testing::_))
		.WillOnce(testing::DoAll(
				SetArgNPointeeTo<0>(correct_answer),
				testing::Return(correct_answer.length()))
				)
		.WillOnce(testing::DoAll(
				SetArgNPointeeTo<0>(correct_event),
				testing::Return(correct_event.length()))
				);

	EXPECT_CALL(serial_dev, available())
		.WillOnce(testing::Return(10));

	EXPECT_TRUE(aura_chip_uut.link());
}

TEST(aurachip_ut, link_cmd_fail)
{
	const std::string device_port{"COM6"};
	aura::chip aura_chip_uut{device_port};
	auto& serial_dev = aura_chip_uut.get_connection().get_serial_dev();
	TimeoutRAII timeout_raii;
	auto& timeout_instance = timeout_raii.get_instance();
	const std::string wrong_answer{
			"AT:START\r\n"
			"SOURCE:COMMAND\r\n"
			"COMMAND: LINK\r\n"
			"STATUS:ERROR\r\n"
			"AT:STOP\r\n"
	};

	EXPECT_CALL(timeout_instance, sleep_for_ms(testing::_))
		.Times(1);

	EXPECT_CALL(serial_dev, flushReceiver())
		.Times(1);

	EXPECT_CALL(serial_dev, writeBytes(StrEqVoidPointer("AT+LINK\n"), testing::_))
		.Times(1);

	EXPECT_CALL(serial_dev, readBytes(testing::_, testing::_, testing::_, testing::_))
		.WillOnce(testing::DoAll(
				SetArgNPointeeTo<0>(wrong_answer),
				testing::Return(wrong_answer.length()))
				);

	EXPECT_FALSE(aura_chip_uut.link());
}

TEST(aurachip_ut, link_event_missing)
{
	const std::string device_port{"COM6"};
	aura::chip aura_chip_uut{device_port};
	auto& serial_dev = aura_chip_uut.get_connection().get_serial_dev();
	TimeoutRAII timeout_raii;
	auto& timeout_instance = timeout_raii.get_instance();
	const std::string correct_answer{
		"AT:START\r\n"
		"SOURCE:COMMAND\r\n"
		"COMMAND: LINK\r\n"
		"STATUS:OK\r\n"
		"AT:STOP\r\n"
		};

	EXPECT_CALL(timeout_instance, sleep_for_ms(testing::_))
		.WillRepeatedly(testing::Return());

	EXPECT_CALL(serial_dev, flushReceiver())
		.Times(1);

	EXPECT_CALL(serial_dev, writeBytes(StrEqVoidPointer("AT+LINK\n"), testing::_))
		.Times(1);

	EXPECT_CALL(serial_dev, readBytes(testing::_, testing::_, testing::_, testing::_))
		.WillOnce(testing::DoAll(
				SetArgNPointeeTo<0>(correct_answer),
				testing::Return(correct_answer.length()))
				);

	EXPECT_CALL(serial_dev, available())
		.WillRepeatedly(testing::Return(0));

	EXPECT_FALSE(aura_chip_uut.link());
}

TEST(aurachip_ut, link_event_fail)
{
	const std::string device_port{"COM6"};
	aura::chip aura_chip_uut{device_port};
	auto& serial_dev = aura_chip_uut.get_connection().get_serial_dev();
	TimeoutRAII timeout_raii;
	auto& timeout_instance = timeout_raii.get_instance();
	const std::string correct_answer{
		"AT:START\r\n"
		"SOURCE:COMMAND\r\n"
		"COMMAND: LINK\r\n"
		"STATUS:OK\r\n"
		"AT:STOP\r\n"
		};
	const std::string error_event{
		"AT:START\r\n"
		"SOURCE:EVENT\r\n"
		"EVENT:LINK\r\n"
		"STATUS:ERROR\r\n"
		"AT:STOP\r\n"
		};

	EXPECT_CALL(timeout_instance, sleep_for_ms(testing::_))
		.Times(1);

	EXPECT_CALL(serial_dev, flushReceiver())
		.Times(1);

	EXPECT_CALL(serial_dev, writeBytes(StrEqVoidPointer("AT+LINK\n"), testing::_))
		.Times(1);

	EXPECT_CALL(serial_dev, readBytes(testing::_, testing::_, testing::_, testing::_))
		.WillOnce(testing::DoAll(
				SetArgNPointeeTo<0>(correct_answer),
				testing::Return(correct_answer.length()))
				)
		.WillOnce(testing::DoAll(
				SetArgNPointeeTo<0>(error_event),
				testing::Return(error_event.length()))
				);

	EXPECT_CALL(serial_dev, available())
		.WillOnce(testing::Return(10));

	EXPECT_FALSE(aura_chip_uut.link());
}

TEST(aurachip_ut, factory_reset_ok)
{
	const std::string correct_answer {
		"AT:START\r\n"
		"SOURCE:COMMAND\r\n"
		"COMMAND: MSTRST\r\n"
		"STATUS:OK\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:1\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:2\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:3\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:4\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:5\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:6\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:7\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:8\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:9\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:10\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:11\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:12\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:13\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:14\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:15\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:16\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:17\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:18\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:19\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:20\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:21\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:22\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:23\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:24\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:25\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:26\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:27\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:28\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:29\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:30\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:31\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:32\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:33\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:34\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:35\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:36\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:37\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:38\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:39\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:40\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:41\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:42\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:43\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:44\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:45\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:46\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:47\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:48\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:49\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:50\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:51\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:52\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:53\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:54\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:55\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:56\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:57\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:58\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:59\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:60\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:61\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:62\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:63\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:64\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:65\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:66\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:67\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:68\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:69\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:70\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:71\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:72\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:73\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:74\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:75\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:76\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:77\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:78\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:79\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:80\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:81\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:82\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:83\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:84\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:85\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:86\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:87\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:88\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:89\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:90\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:91\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:92\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:93\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:94\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:95\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:96\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:97\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:98\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:99\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:100\r\n"
		"AT:STOP\r\n"
	};

	const std::string device_port{"COM6"};
	aura::chip aura_chip_uut{device_port};
	auto& serial_dev = aura_chip_uut.get_connection().get_serial_dev();
	TimeoutRAII timeout_raii;
	auto& timeout_instance = timeout_raii.get_instance();

	EXPECT_CALL(timeout_instance, sleep_for_ms(aura::chip::FACTORY_RESET_WAIT_MS))
		.Times(1);

	EXPECT_CALL(serial_dev, flushReceiver())
		.Times(1);

	EXPECT_CALL(serial_dev, writeBytes(StrEqVoidPointer("AT+MSTRST\n"), testing::_))
		.Times(1);

	EXPECT_CALL(serial_dev, readBytes(testing::_, aura::chip::FACTORY_RESET_BUFFER_SIZE, testing::_, testing::_))
		.WillOnce(testing::DoAll(
				SetArgNPointeeTo<0>(correct_answer),
				testing::Return(correct_answer.length()))
				);

	EXPECT_TRUE(aura_chip_uut.factory_reset());
}

TEST(aurachip_ut, factory_reset_failed)
{
	const std::string incorrect_answer_clear_failed {
		"AT:START\r\n"
		"SOURCE:COMMAND\r\n"
		"COMMAND: MSTRST\r\n"
		"STATUS:ERROR\r\n"
		"AT:STOP\r\n"
	};
	const std::string device_port{"COM6"};
	aura::chip aura_chip_uut{device_port};
	auto& serial_dev = aura_chip_uut.get_connection().get_serial_dev();
	TimeoutRAII timeout_raii;
	auto& timeout_instance = timeout_raii.get_instance();

	EXPECT_CALL(timeout_instance, sleep_for_ms(aura::chip::FACTORY_RESET_WAIT_MS))
		.Times(1);

	EXPECT_CALL(serial_dev, flushReceiver())
		.Times(1);

	EXPECT_CALL(serial_dev, writeBytes(testing::_, testing::_))
		.Times(1);

	EXPECT_CALL(serial_dev, readBytes(testing::_, aura::chip::FACTORY_RESET_BUFFER_SIZE, testing::_, testing::_))
		.WillOnce(testing::DoAll(
				SetArgNPointeeTo<0>(incorrect_answer_clear_failed),
				testing::Return(incorrect_answer_clear_failed.length()))
				);

	EXPECT_FALSE(aura_chip_uut.factory_reset());
}

TEST(aurachip_ut, factory_reset_clear_failed)
{
	const std::string incorrect_answer_clear_failed {
		"AT:START\r\n"
		"SOURCE:COMMAND\r\n"
		"COMMAND: MSTRST\r\n"
		"STATUS:OK\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:1\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:2\r\n"
		"AT:STOP\r\n"
		"AT:START\r\n"
		"SOURCE:CLEAR\r\n"
		"STATUS:OK\r\n"
		"PROGRESS:3\r\n"
		"AT:STOP\r\n"
	};
	const std::string device_port{"COM6"};
	aura::chip aura_chip_uut{device_port};
	auto& serial_dev = aura_chip_uut.get_connection().get_serial_dev();
	TimeoutRAII timeout_raii;
	auto& timeout_instance = timeout_raii.get_instance();

	EXPECT_CALL(timeout_instance, sleep_for_ms(aura::chip::FACTORY_RESET_WAIT_MS))
		.Times(1);

	EXPECT_CALL(serial_dev, flushReceiver())
		.Times(1);

	EXPECT_CALL(serial_dev, writeBytes(testing::_, testing::_))
		.Times(1);

	EXPECT_CALL(serial_dev, readBytes(testing::_, aura::chip::FACTORY_RESET_BUFFER_SIZE, testing::_, testing::_))
		.WillOnce(testing::DoAll(
				SetArgNPointeeTo<0>(incorrect_answer_clear_failed),
				testing::Return(incorrect_answer_clear_failed.length()))
				);

	EXPECT_FALSE(aura_chip_uut.factory_reset());
}

TEST(aurachip_ut, factory_reset_not_ok)
{
	const std::string incorrect_answer_not_ok {
			"Clear:1%\r\n"
			"Clear:2%\r\n"
			"Clear:3%\r\n"
			"Clear:4%\r\n"
			"Clear:5%\r\n"
			"Clear:6%\r\n"
			"Clear:7%\r\n"
			"Clear:8%\r\n"
			"Clear:9%\r\n"
			"Clear:10%\r\n"
			"Clear:11%\r\n"
			"Clear:12%\r\n"
			"Clear:13%\r\n"
			"Clear:14%\r\n"
			"Clear:15%\r\n"
			"Clear:16%\r\n"
			"Clear:17%\r\n"
			"Clear:18%\r\n"
			"Clear:19%\r\n"
			"Clear:20%\r\n"
			"Clear:21%\r\n"
			"Clear:22%\r\n"
			"Clear:23%\r\n"
			"Clear:24%\r\n"
			"Clear:25%\r\n"
			"Clear:26%\r\n"
			"Clear:27%\r\n"
			"Clear:28%\r\n"
			"Clear:29%\r\n"
			"Clear:30%\r\n"
			"Clear:31%\r\n"
			"Clear:32%\r\n"
			"Clear:33%\r\n"
			"Clear:34%\r\n"
			"Clear:35%\r\n"
			"Clear:36%\r\n"
			"Clear:37%\r\n"
			"Clear:38%\r\n"
			"Clear:39%\r\n"
			"Clear:40%\r\n"
			"Clear:41%\r\n"
			"Clear:42%\r\n"
			"Clear:43%\r\n"
			"Clear:44%\r\n"
			"Clear:45%\r\n"
			"Clear:46%\r\n"
			"Clear:47%\r\n"
			"Clear:48%\r\n"
			"Clear:49%\r\n"
			"Clear:50%\r\n"
			"Clear:51%\r\n"
			"Clear:52%\r\n"
			"Clear:53%\r\n"
			"Clear:54%\r\n"
			"Clear:55%\r\n"
			"Clear:56%\r\n"
			"Clear:57%\r\n"
			"Clear:58%\r\n"
			"Clear:59%\r\n"
			"Clear:60%\r\n"
			"Clear:61%\r\n"
			"Clear:62%\r\n"
			"Clear:63%\r\n"
			"Clear:64%\r\n"
			"Clear:65%\r\n"
			"Clear:66%\r\n"
			"Clear:67%\r\n"
			"Clear:68%\r\n"
			"Clear:69%\r\n"
			"Clear:70%\r\n"
			"Clear:71%\r\n"
			"Clear:72%\r\n"
			"Clear:73%\r\n"
			"Clear:74%\r\n"
			"Clear:75%\r\n"
			"Clear:76%\r\n"
			"Clear:77%\r\n"
			"Clear:78%\r\n"
			"Clear:79%\r\n"
			"Clear:80%\r\n"
			"Clear:81%\r\n"
			"Clear:82%\r\n"
			"Clear:83%\r\n"
			"Clear:84%\r\n"
			"Clear:85%\r\n"
			"Clear:86%\r\n"
			"Clear:87%\r\n"
			"Clear:88%\r\n"
			"Clear:89%\r\n"
			"Clear:90%\r\n"
			"Clear:91%\r\n"
			"Clear:92%\r\n"
			"Clear:93%\r\n"
			"Clear:94%\r\n"
			"Clear:95%\r\n"
			"Clear:96%\r\n"
			"Clear:97%\r\n"
			"Clear:98%\r\n"
			"Clear:99%\r\n"
			"Clear:100%\r\n"
	};
	const std::string device_port{"COM6"};
	aura::chip aura_chip_uut{device_port};
	auto& serial_dev = aura_chip_uut.get_connection().get_serial_dev();
	auto& timeout_instance = Timeout::get_instance();

	EXPECT_CALL(timeout_instance, sleep_for_ms(aura::chip::FACTORY_RESET_WAIT_MS))
		.Times(1);

	EXPECT_CALL(serial_dev, flushReceiver())
		.Times(1);

	EXPECT_CALL(serial_dev, writeBytes(testing::_, testing::_))
		.Times(1);

	EXPECT_CALL(serial_dev, readBytes(testing::_, aura::chip::FACTORY_RESET_BUFFER_SIZE, testing::_, testing::_))
		.WillOnce(testing::DoAll(
				SetArgNPointeeTo<0>(incorrect_answer_not_ok),
				testing::Return(incorrect_answer_not_ok.length()))
				);

	EXPECT_FALSE(aura_chip_uut.factory_reset());
}

TEST(aurachip_ut, update_device_parameters_ok)
{
	const std::string correct_answer {
			"AT:START\r\n"
			"SOURCE:COMMAND\r\n"
			"COMMAND: GETDEVOPTION?\r\n"
			"STATUS:OK\r\n"
			"ID: 3\r\n"
			"ADDRESS: 30090005\r\n"
			"PCODE: 3009\r\n"
			"FVER: 1.11\r\n"
			"HVER: 28.1\r\n"
			"MANCODE: 37\r\n"
			"CODE: 32\r\n"
			"CHANNEL: 0\r\n"
			"FLAG OWN: 0\r\n"
			"FLAG WRITEABLE: 1\r\n"
			"VALUE: 178\r\n"
			"CODE: 101\r\n"
			"CHANNEL: 0\r\n"
			"FLAG OWN: 0\r\n"
			"FLAG WRITEABLE: 0\r\n"
			"VALUE: 1927\r\n"
			"AT:STOP\r\n"
	};

	const std::string device_str {
			"AT:START\r\n"
			"SOURCE:COMMAND\r\n"
			"COMMAND: LIST?\r\n"
			"STATUS:OK\r\n"
			"ID: 3\r\n"
			"ADDRESS: 30090005\r\n"
			"PCODE: 3009\r\n"
			"FVER: 1.11\r\n"
			"HVER: 28.1\r\n"
			"MANCODE: 37\r\n"
			"AT:STOP\r\n"
	};

	const std::vector<aura::parameter> test_parameters {
			aura::parameter{ 32, 0, 0, 1 , 178 },
			aura::parameter{ 101, 0, 0, 0, 1927 }
	};
	const int32_t dev_id = 3;
	const std::string device_port{"COM6"};
	aura::chip aura_chip_uut{device_port};
	auto& serial_dev = aura_chip_uut.get_connection().get_serial_dev();
	TimeoutRAII timeout_raii;
	auto& timeout_instance = timeout_raii.get_instance();
	const std::string get_dev_parameters_cmd {"AT+GETDEVOPTION=" + std::to_string(dev_id) + "\n"};

	EXPECT_CALL(timeout_instance, sleep_for_ms(testing::_))
		.Times(2);

	EXPECT_CALL(serial_dev, flushReceiver())
		.Times(2);

	EXPECT_CALL(serial_dev, writeBytes(testing::_, testing::_))
		.Times(1);

	EXPECT_CALL(serial_dev, writeBytes(StrEqVoidPointer(get_dev_parameters_cmd), testing::_))
		.Times(1);

	EXPECT_CALL(serial_dev, readBytes(testing::_, testing::_, testing::_, testing::_))
		.WillOnce(testing::DoAll(
				SetArgNPointeeTo<0>(device_str),
				testing::Return(device_str.length()))
				);

	aura_chip_uut.update_device_list();
	EXPECT_CALL(serial_dev, readBytes(testing::_, testing::_, testing::_, testing::_))
		.WillOnce(testing::DoAll(
				SetArgNPointeeTo<0>(correct_answer),
				testing::Return(correct_answer.length()))
				);
	EXPECT_TRUE(aura_chip_uut.update_device_parameters(dev_id));

	EXPECT_EQ(aura_chip_uut.get_device(dev_id).get_parameters().size(), test_parameters.size());

	for (const auto& parameter : test_parameters) {
		const auto &read_parameter = aura_chip_uut.get_device(dev_id).get_parameter(parameter.get_code());
		EXPECT_EQ(parameter, read_parameter);
	}
}

TEST(aurachip_ut, update_device_parameters_failed_id)
{
	const std::string correct_answer {
			"AT:START\r\n"
			"SOURCE:COMMAND\r\n"
			"COMMAND: GETDEVOPTION?\r\n"
			"STATUS:OK\r\n"
			"ID: 31\r\n"
			"ADDRESS: 30090005\r\n"
			"PCODE: 3009\r\n"
			"FVER: 1.11\r\n"
			"HVER: 28.1\r\n"
			"MANCODE: 37\r\n"
			"CODE: 32\r\n"
			"CHANNEL: 0\r\n"
			"FLAG OWN: 0\r\n"
			"FLAG WRITEABLE: 1\r\n"
			"VALUE: 178\r\n"
			"CODE: 101\r\n"
			"CHANNEL: 0\r\n"
			"FLAG OWN: 0\r\n"
			"FLAG WRITEABLE: 0\r\n"
			"VALUE: 1927\r\n"
			"AT:STOP\r\n"
	};

	const int32_t dev_id = 3;
	const std::string device_port{"COM6"};
	aura::chip aura_chip_uut{device_port};
	auto& serial_dev = aura_chip_uut.get_connection().get_serial_dev();
	TimeoutRAII timeout_raii;
	auto& timeout_instance = timeout_raii.get_instance();

	EXPECT_CALL(timeout_instance, sleep_for_ms(testing::_))
		.Times(1);

	EXPECT_CALL(serial_dev, flushReceiver())
		.Times(1);

	EXPECT_CALL(serial_dev, writeBytes(testing::_, testing::_))
		.Times(1);

	EXPECT_CALL(serial_dev, readBytes(testing::_, testing::_, testing::_, testing::_))
		.WillOnce(testing::DoAll(
				SetArgNPointeeTo<0>(correct_answer),
				testing::Return(correct_answer.length()))
				);
	EXPECT_FALSE(aura_chip_uut.update_device_parameters(dev_id));
}

TEST(aurachip_ut, update_device_parameters_device_list_failed)
{
	const std::string correct_answer {
			"AT:START\r\n"
			"SOURCE:COMMAND\r\n"
			"COMMAND: GETDEVOPTION?\r\n"
			"STATUS:OK\r\n"
			"ID: 3\r\n"
			"ADDRESS: 30090005\r\n"
			"PCODE: 3009\r\n"
			"FVER: 1.11\r\n"
			"HVER: 28.1\r\n"
			"MANCODE: 37\r\n"
			"CODE: 32\r\n"
			"CHANNEL: 0\r\n"
			"FLAG OWN: 0\r\n"
			"FLAG WRITEABLE: 1\r\n"
			"VALUE: 178\r\n"
			"CODE: 101\r\n"
			"CHANNEL: 0\r\n"
			"FLAG OWN: 0\r\n"
			"FLAG WRITEABLE: 0\r\n"
			"VALUE: 1927\r\n"
			"AT:STOP\r\n"
	};

	const int32_t dev_id = 3;
	const std::string device_port{"COM6"};
	aura::chip aura_chip_uut{device_port};
	auto& serial_dev = aura_chip_uut.get_connection().get_serial_dev();
	auto& timeout_instance = Timeout::get_instance();

	EXPECT_CALL(timeout_instance, sleep_for_ms(testing::_))
		.Times(1);

	EXPECT_CALL(serial_dev, flushReceiver())
		.Times(1);

	EXPECT_CALL(serial_dev, writeBytes(testing::_, testing::_))
		.Times(1);

	EXPECT_CALL(serial_dev, readBytes(testing::_, testing::_, testing::_, testing::_))
		.WillOnce(testing::DoAll(
				SetArgNPointeeTo<0>(correct_answer),
				testing::Return(correct_answer.length()))
				);
	EXPECT_FALSE(aura_chip_uut.update_device_parameters(dev_id));
}

TEST(aurachip_ut, update_device_parameters_wrong_device)
{
	const std::string correct_answer {
			"AT:START\r\n"
			"SOURCE:COMMAND\r\n"
			"COMMAND: GETDEVOPTION?\r\n"
			"STATUS:OK\r\n"
			"ID: 3\r\n"
			"ADDRESS: 30090005\r\n"
			"PCODE: 3009\r\n"
			"FVER: 1.11\r\n"
			"HVER: 28.1\r\n"
			"MANCODE: 37\r\n"
			"CODE: 32\r\n"
			"CHANNEL: 0\r\n"
			"FLAG OWN: 0\r\n"
			"FLAG WRITEABLE: 1\r\n"
			"VALUE: 178\r\n"
			"CODE: 101\r\n"
			"CHANNEL: 0\r\n"
			"FLAG OWN: 0\r\n"
			"FLAG WRITEABLE: 0\r\n"
			"VALUE: 1927\r\n"
			"AT:STOP\r\n"
	};

	const std::string device_str {
			"AT:START\r\n"
			"SOURCE:COMMAND\r\n"
			"COMMAND: LIST?\r\n"
			"STATUS:OK\r\n"
			"ID: 3\r\n"
			"ADDRESS: 222222\r\n"
			"PCODE: 3009\r\n"
			"FVER: 1.11\r\n"
			"HVER: 28.1\r\n"
			"MANCODE: 37\r\n"
			"AT:STOP\r\n"
	};

	const int32_t dev_id = 3;
	const std::string device_port{"COM6"};
	aura::chip aura_chip_uut{device_port};
	auto& serial_dev = aura_chip_uut.get_connection().get_serial_dev();
	TimeoutRAII timeout_raii;
	auto& timeout_instance = timeout_raii.get_instance();

	EXPECT_CALL(timeout_instance, sleep_for_ms(testing::_))
		.Times(1);

	EXPECT_CALL(serial_dev, flushReceiver())
		.Times(1);

	EXPECT_CALL(serial_dev, writeBytes(testing::_, testing::_))
		.Times(1);

	EXPECT_CALL(serial_dev, readBytes(testing::_, testing::_, testing::_, testing::_))
		.WillOnce(testing::DoAll(
				SetArgNPointeeTo<0>(device_str),
				testing::Return(device_str.length()))
				);

	aura_chip_uut.update_device_list();

	EXPECT_CALL(timeout_instance, sleep_for_ms(testing::_))
		.Times(1);

	EXPECT_CALL(serial_dev, flushReceiver())
		.Times(1);

	EXPECT_CALL(serial_dev, writeBytes(testing::_, testing::_))
		.Times(1);

	EXPECT_CALL(serial_dev, readBytes(testing::_, testing::_, testing::_, testing::_))
		.WillOnce(testing::DoAll(
				SetArgNPointeeTo<0>(correct_answer),
				testing::Return(correct_answer.length()))
				);
	EXPECT_FALSE(aura_chip_uut.update_device_parameters(dev_id));
}

TEST(aurachip_ut, update_device_parameter_ok)
{
	const int32_t code = 45;
	const int32_t dev_id = 3;

	const std::string device_parameters = {
			"AT:START\r\n"
			"SOURCE:COMMAND\r\n"
			"COMMAND: GETDEVOPTION?\r\n"
			"STATUS:OK\r\n"
			"ID: 3\r\n"
			"ADDRESS: 30090005\r\n"
			"PCODE: 3009\r\n"
			"FVER: 1.11\r\n"
			"HVER: 28.1\r\n"
			"MANCODE: 37\r\n"
			"CODE: 45\r\n"
			"CHANNEL: 0\r\n"
			"FLAG OWN: 0\r\n"
			"FLAG WRITEABLE: 1\r\n"
			"VALUE: 178\r\n"
			"CODE: 101\r\n"
			"CHANNEL: 0\r\n"
			"FLAG OWN: 0\r\n"
			"FLAG WRITEABLE: 0\r\n"
			"VALUE: 1927\r\n"
			"AT:STOP\r\n"
	};

	const std::string new_parameter = {
			"AT:START\r\n"
			"SOURCE:COMMAND\r\n"
			"COMMAND: GETDEVCODEOPTION?\r\n"
			"STATUS:OK\r\n"
			"ID: 3\r\n"
			"ADDRESS: 30090005\r\n"
			"PCODE: 3009\r\n"
			"FVER: 1.11\r\n"
			"HVER: 28.1\r\n"
			"MANCODE: 37\r\n"
			"CODE: 45\r\n"
			"CHANNEL: 2\r\n"
			"FLAG OWN: 1\r\n"
			"FLAG WRITEABLE: 0\r\n"
			"VALUE: 202\r\n"
			"AT:STOP\r\n"
	};

	const std::string device_str = {
			"AT:START\r\n"
			"SOURCE:COMMAND\r\n"
			"COMMAND: LIST?\r\n"
			"STATUS:OK\r\n"
			"ID: 3\r\n"
			"ADDRESS: 30090005\r\n"
			"PCODE: 3009\r\n"
			"FVER: 1.11\r\n"
			"HVER: 28.1\r\n"
			"MANCODE: 37\r\n"
			"AT:STOP\r\n"
	};

	const aura::parameter test_parameter{ code, 2, 1, 0 , 202 };
	const std::string device_port{"COM6"};
	aura::chip aura_chip_uut{device_port};
	auto& serial_dev = aura_chip_uut.get_connection().get_serial_dev();
	TimeoutRAII timeout_raii;
	auto& timeout_instance = timeout_raii.get_instance();

	EXPECT_CALL(timeout_instance, sleep_for_ms(testing::_))
		.Times(3);

	EXPECT_CALL(serial_dev, flushReceiver())
		.Times(3);

	EXPECT_CALL(serial_dev, writeBytes(testing::_, testing::_))
		.Times(3);

	EXPECT_CALL(serial_dev, readBytes(testing::_, testing::_, testing::_, testing::_))
		.WillOnce(testing::DoAll(
				SetArgNPointeeTo<0>(device_str),
				testing::Return(device_str.length())))
		.WillOnce(testing::DoAll(
				SetArgNPointeeTo<0>(device_parameters),
				testing::Return(device_parameters.length()))
				);

	aura_chip_uut.update_device_list();
	aura_chip_uut.update_device_parameters(dev_id);

	EXPECT_CALL(serial_dev, readBytes(testing::_,testing::_, testing::_, testing::_))
		.WillOnce(testing::DoAll(
				SetArgNPointeeTo<0>(new_parameter),
				testing::Return(new_parameter.length()))
				);
	EXPECT_TRUE(aura_chip_uut.update_device_parameter(dev_id, code));
	EXPECT_EQ(aura_chip_uut.get_device(dev_id).get_parameter(code), test_parameter);
}

TEST(aurachip_ut, update_device_parameter_wrong_dev)
{
	const int32_t code = 45;
	const int32_t dev_id = 3;

	const std::string device_parameters = {
			"AT:START\r\n"
			"SOURCE:COMMAND\r\n"
			"COMMAND: GETDEVOPTION?\r\n"
			"STATUS:OK\r\n"
			"ID: 3\r\n"
			"ADDRESS: 30090005\r\n"
			"PCODE: 3009\r\n"
			"FVER: 1.11\r\n"
			"HVER: 28.1\r\n"
			"MANCODE: 37\r\n"
			"CODE: 45\r\n"
			"CHANNEL: 0\r\n"
			"FLAG OWN: 0\r\n"
			"FLAG WRITEABLE: 1\r\n"
			"VALUE: 178\r\n"
			"CODE: 101\r\n"
			"CHANNEL: 0\r\n"
			"FLAG OWN: 0\r\n"
			"FLAG WRITEABLE: 0\r\n"
			"VALUE: 1927\r\n"
			"AT:STOP\r\n"
	};

	const std::string new_parameter = {
			"AT:START\r\n"
			"SOURCE:COMMAND\r\n"
			"COMMAND: GETDEVCODEOPTION?\r\n"
			"STATUS:OK\r\n"
			"ID: 3\r\n"
			"ADDRESS: 30090005\r\n"
			"PCODE: 4010\r\n"
			"FVER: 1.11\r\n"
			"HVER: 28.1\r\n"
			"MANCODE: 37\r\n"
			"CODE: 45\r\n"
			"CHANNEL: 2\r\n"
			"FLAG OWN: 1\r\n"
			"FLAG WRITEABLE: 0\r\n"
			"VALUE: 202\r\n"
			"AT:STOP\r\n"
	};

	const std::string device_str = {
			"AT:START\r\n"
			"SOURCE:COMMAND\r\n"
			"COMMAND: LIST?\r\n"
			"STATUS:OK\r\n"
			"ID: 3\r\n"
			"ADDRESS: 30090005\r\n"
			"PCODE: 3009\r\n"
			"FVER: 1.11\r\n"
			"HVER: 28.1\r\n"
			"MANCODE: 37\r\n"
			"AT:STOP\r\n"
	};

	const aura::parameter test_parameter{ code, 2, 1, 0 , 202 };
	const std::string device_port{"COM6"};
	aura::chip aura_chip_uut{device_port};
	auto& serial_dev = aura_chip_uut.get_connection().get_serial_dev();
	TimeoutRAII timeout_raii;
	auto& timeout_instance = timeout_raii.get_instance();

	EXPECT_CALL(timeout_instance, sleep_for_ms(testing::_))
		.Times(3);

	EXPECT_CALL(serial_dev, flushReceiver())
		.Times(3);

	EXPECT_CALL(serial_dev, writeBytes(testing::_, testing::_))
		.Times(3);

	EXPECT_CALL(serial_dev, readBytes(testing::_, testing::_, testing::_, testing::_))
		.WillOnce(testing::DoAll(
				SetArgNPointeeTo<0>(device_str),
				testing::Return(device_str.length())))
		.WillOnce(testing::DoAll(
				SetArgNPointeeTo<0>(device_parameters),
				testing::Return(device_parameters.length()))
				);

	aura_chip_uut.update_device_list();
	aura_chip_uut.update_device_parameters(dev_id);

	EXPECT_CALL(serial_dev, readBytes(testing::_, testing::_, testing::_, testing::_))
		.WillOnce(testing::DoAll(
				SetArgNPointeeTo<0>(new_parameter),
				testing::Return(new_parameter.length()))
				);
	EXPECT_FALSE(aura_chip_uut.update_device_parameter(dev_id, code));
}

TEST(aurachip_ut, update_device_parameter_missing_param)
{
	const int32_t code = 45;
	const int32_t dev_id = 3;

	const std::string device_parameters = {
			"AT:START\r\n"
			"SOURCE:COMMAND\r\n"
			"COMMAND: GETDEVOPTION?\r\n"
			"STATUS:OK\r\n"
			"ID: 3\r\n"
			"ADDRESS: 30090005\r\n"
			"PCODE: 3009\r\n"
			"FVER: 1.11\r\n"
			"HVER: 28.1\r\n"
			"MANCODE: 37\r\n"
			"CODE: 45\r\n"
			"CHANNEL: 0\r\n"
			"FLAG OWN: 0\r\n"
			"FLAG WRITEABLE: 1\r\n"
			"VALUE: 178\r\n"
			"CODE: 101\r\n"
			"CHANNEL: 0\r\n"
			"FLAG OWN: 0\r\n"
			"FLAG WRITEABLE: 0\r\n"
			"VALUE: 1927\r\n"
			"AT:STOP\r\n"
	};

	const std::string new_parameter = {
			"AT:START\r\n"
			"SOURCE:COMMAND\r\n"
			"COMMAND: GETDEVCODEOPTION?\r\n"
			"STATUS:OK\r\n"
			"ID: 3\r\n"
			"ADDRESS: 30090005\r\n"
			"PCODE: 3009\r\n"
			"FVER: 1.11\r\n"
			"HVER: 28.1\r\n"
			"MANCODE: 37\r\n"
			"CODX: 45\r\n"
			"CHANNEL: 2\r\n"
			"FLAG OWN: 1\r\n"
			"FLAG WRITEABLE: 0\r\n"
			"VALUE: 202\r\n"
			"AT:STOP\r\n"
	};

	const std::string device_str = {
			"AT:START\r\n"
			"SOURCE:COMMAND\r\n"
			"COMMAND: LIST?\r\n"
			"STATUS:OK\r\n"
			"ID: 3\r\n"
			"ADDRESS: 30090005\r\n"
			"PCODE: 3009\r\n"
			"FVER: 1.11\r\n"
			"HVER: 28.1\r\n"
			"MANCODE: 37\r\n"
			"AT:STOP\r\n"
	};

	const aura::parameter test_parameter{ code, 2, 1, 0 , 202 };
	const std::string device_port{"COM6"};
	aura::chip aura_chip_uut{device_port};
	auto& serial_dev = aura_chip_uut.get_connection().get_serial_dev();
	TimeoutRAII timeout_raii;
	auto& timeout_instance = timeout_raii.get_instance();

	EXPECT_CALL(timeout_instance, sleep_for_ms(testing::_))
		.Times(3);

	EXPECT_CALL(serial_dev, flushReceiver())
		.Times(3);

	EXPECT_CALL(serial_dev, writeBytes(testing::_, testing::_))
		.Times(3);

	EXPECT_CALL(serial_dev, readBytes(testing::_, testing::_, testing::_, testing::_))
		.WillOnce(testing::DoAll(
				SetArgNPointeeTo<0>(device_str),
				testing::Return(device_str.length())))
		.WillOnce(testing::DoAll(
				SetArgNPointeeTo<0>(device_parameters),
				testing::Return(device_parameters.length()))
				);

	aura_chip_uut.update_device_list();
	aura_chip_uut.update_device_parameters(dev_id);

	EXPECT_CALL(serial_dev, readBytes(testing::_, testing::_, testing::_, testing::_))
		.WillOnce(testing::DoAll(
				SetArgNPointeeTo<0>(new_parameter),
				testing::Return(new_parameter.length()))
				);
	EXPECT_FALSE(aura_chip_uut.update_device_parameter(dev_id, code));
}

TEST(aurachip_ut, update_device_parameter_wrong_code)
{
	const int32_t code = 45;
	const int32_t dev_id = 3;

	const std::string device_parameters = {
			"AT:START\r\n"
			"SOURCE:COMMAND\r\n"
			"COMMAND: GETDEVOPTION?\r\n"
			"STATUS:OK\r\n"
			"ID: 3\r\n"
			"ADDRESS: 30090005\r\n"
			"PCODE: 3009\r\n"
			"FVER: 1.11\r\n"
			"HVER: 28.1\r\n"
			"MANCODE: 37\r\n"
			"CODE: 45\r\n"
			"CHANNEL: 0\r\n"
			"FLAG OWN: 0\r\n"
			"FLAG WRITEABLE: 1\r\n"
			"VALUE: 178\r\n"
			"CODE: 101\r\n"
			"CHANNEL: 0\r\n"
			"FLAG OWN: 0\r\n"
			"FLAG WRITEABLE: 0\r\n"
			"VALUE: 1927\r\n"
			"AT:STOP\r\n"
	};

	const std::string new_parameter = {
			"AT:START\r\n"
			"SOURCE:COMMAND\r\n"
			"COMMAND: GETDEVCODEOPTION?\r\n"
			"STATUS:OK\r\n"
			"ID: 3\r\n"
			"ADDRESS: 30090005\r\n"
			"PCODE: 3009\r\n"
			"FVER: 1.11\r\n"
			"HVER: 28.1\r\n"
			"MANCODE: 37\r\n"
			"CODE: 47\r\n"
			"CHANNEL: 2\r\n"
			"FLAG OWN: 1\r\n"
			"FLAG WRITEABLE: 0\r\n"
			"VALUE: 202\r\n"
			"AT:STOP\r\n"
	};

	const std::string device_str = {
			"AT:START\r\n"
			"SOURCE:COMMAND\r\n"
			"COMMAND: LIST?\r\n"
			"STATUS:OK\r\n"
			"ID: 3\r\n"
			"ADDRESS: 30090005\r\n"
			"PCODE: 3009\r\n"
			"FVER: 1.11\r\n"
			"HVER: 28.1\r\n"
			"MANCODE: 37\r\n"
			"AT:STOP\r\n"
	};

	const aura::parameter test_parameter{ code, 2, 1, 0 , 202 };
	const std::string device_port{"COM6"};
	aura::chip aura_chip_uut{device_port};
	auto& serial_dev = aura_chip_uut.get_connection().get_serial_dev();
	TimeoutRAII timeout_raii;
	auto& timeout_instance = timeout_raii.get_instance();

	EXPECT_CALL(timeout_instance, sleep_for_ms(testing::_))
		.Times(3);

	EXPECT_CALL(serial_dev, flushReceiver())
		.Times(3);

	EXPECT_CALL(serial_dev, writeBytes(testing::_, testing::_))
		.Times(3);

	EXPECT_CALL(serial_dev, readBytes(testing::_, testing::_, testing::_, testing::_))
		.WillOnce(testing::DoAll(
				SetArgNPointeeTo<0>(device_str),
				testing::Return(device_str.length())))
		.WillOnce(testing::DoAll(
				SetArgNPointeeTo<0>(device_parameters),
				testing::Return(device_parameters.length()))
				);

	aura_chip_uut.update_device_list();
	aura_chip_uut.update_device_parameters(dev_id);

	EXPECT_CALL(serial_dev, readBytes(testing::_, testing::_, testing::_, testing::_))
		.WillOnce(testing::DoAll(
				SetArgNPointeeTo<0>(new_parameter),
				testing::Return(new_parameter.length()))
				);
	EXPECT_FALSE(aura_chip_uut.update_device_parameter(dev_id, code));
}

TEST(aurachip_ut, update_device_parameter_to_many_params)
{
	const int32_t code = 45;
	const int32_t dev_id = 3;

	const std::string device_parameters = {
			"AT:START\r\n"
			"SOURCE:COMMAND\r\n"
			"COMMAND: GETDEVOPTION?\r\n"
			"STATUS:OK\r\n"
			"ID: 3\r\n"
			"ADDRESS: 30090005\r\n"
			"PCODE: 3009\r\n"
			"FVER: 1.11\r\n"
			"HVER: 28.1\r\n"
			"MANCODE: 37\r\n"
			"CODE: 45\r\n"
			"CHANNEL: 0\r\n"
			"FLAG OWN: 0\r\n"
			"FLAG WRITEABLE: 1\r\n"
			"VALUE: 178\r\n"
			"CODE: 101\r\n"
			"CHANNEL: 0\r\n"
			"FLAG OWN: 0\r\n"
			"FLAG WRITEABLE: 0\r\n"
			"VALUE: 1927\r\n"
			"AT:STOP\r\n"
	};

	const std::string new_parameter = {
			"AT:START\r\n"
			"SOURCE:COMMAND\r\n"
			"COMMAND: GETDEVCODEOPTION?\r\n"
			"STATUS:OK\r\n"
			"ID: 3\r\n"
			"ADDRESS: 30090005\r\n"
			"PCODE: 3009\r\n"
			"FVER: 1.11\r\n"
			"HVER: 28.1\r\n"
			"MANCODE: 37\r\n"
			"CODE: 45\r\n"
			"VALUE: 202\r\n"
			"CODE: 101\r\n"
			"VALUE: 1927\r\n"
			"AT:STOP\r\n"
	};

	const std::string device_str = {
			"AT:START\r\n"
			"SOURCE:COMMAND\r\n"
			"COMMAND: LIST?\r\n"
			"STATUS:OK\r\n"
			"ID: 3\r\n"
			"ADDRESS: 30090005\r\n"
			"PCODE: 3009\r\n"
			"FVER: 1.11\r\n"
			"HVER: 28.1\r\n"
			"MANCODE: 37\r\n"
			"AT:STOP\r\n"
	};

	const aura::parameter test_parameter{ code, 2, 1, 0 , 202 };
	const std::string device_port{"COM6"};
	aura::chip aura_chip_uut{device_port};
	auto& serial_dev = aura_chip_uut.get_connection().get_serial_dev();
	TimeoutRAII timeout_raii;
	auto& timeout_instance = timeout_raii.get_instance();

	EXPECT_CALL(timeout_instance, sleep_for_ms(testing::_))
		.Times(3);

	EXPECT_CALL(serial_dev, flushReceiver())
		.Times(3);

	EXPECT_CALL(serial_dev, writeBytes(testing::_, testing::_))
		.Times(3);

	EXPECT_CALL(serial_dev, readBytes(testing::_, testing::_, testing::_, testing::_))
		.WillOnce(testing::DoAll(
				SetArgNPointeeTo<0>(device_str),
				testing::Return(device_str.length())))
		.WillOnce(testing::DoAll(
				SetArgNPointeeTo<0>(device_parameters),
				testing::Return(device_parameters.length()))
				);

	aura_chip_uut.update_device_list();
	aura_chip_uut.update_device_parameters(dev_id);

	EXPECT_CALL(serial_dev, readBytes(testing::_, testing::_, testing::_, testing::_))
		.WillOnce(testing::DoAll(
				SetArgNPointeeTo<0>(new_parameter),
				testing::Return(new_parameter.length()))
				);
	EXPECT_FALSE(aura_chip_uut.update_device_parameter(dev_id, code));
}

TEST(aurachip_ut, update_device_list_ok)
{
	const std::string correct_answer {
			"AT:START\r\n"
			"SOURCE:COMMAND\r\n"
			"COMMAND: LIST?\r\n"
			"STATUS:OK\r\n"
			"ID: 1\r\n"
			"ADDRESS: 90FC4F9B\r\n"
			"PCODE: 300d\r\n"
			"FVER: 1.1\r\n"
			"HVER: 7\r\n"
			"MANCODE: 40\r\n"
			"ID: 20\r\n"
			"ADDRESS: 456223DA\r\n"
			"PCODE: 3005\r\n"
			"FVER: 1.5\r\n"
			"HVER: 1\r\n"
			"MANCODE: 30\r\n"
			"AT:STOP\r\n"
	};

	const std::string device_1 {
			"ID: 1\r\n"
			"ADDRESS: 90FC4F9B\r\n"
			"PCODE: 300d\r\n"
			"FVER: 1.1\r\n"
			"HVER: 7\r\n"
			"MANCODE: 40\r\n"
	};
	const std::string device_2 {
			"ID: 20\r\n"
			"ADDRESS: 456223DA\r\n"
			"PCODE: 3005\r\n"
			"FVER: 1.5\r\n"
			"HVER: 1\r\n"
			"MANCODE: 30\r\n"
	};

	const std::map<uint32_t, aura::device> test_devices {
			{ 1, aura::device{ device_1 } },
			{ 20, aura::device{ device_2 } }
	};

	const std::string device_port{"COM6"};
	aura::chip aura_chip_uut{device_port};
	auto& serial_dev = aura_chip_uut.get_connection().get_serial_dev();
	TimeoutRAII timeout_raii;
	auto& timeout_instance = timeout_raii.get_instance();

	EXPECT_CALL(timeout_instance, sleep_for_ms(testing::_))
		.Times(1);

	EXPECT_CALL(serial_dev, flushReceiver())
		.Times(1);

	EXPECT_CALL(serial_dev, writeBytes(StrEqVoidPointer("AT+LIST?\n"), testing::_))
		.Times(1);

	EXPECT_CALL(serial_dev, readBytes(testing::_, testing::_, testing::_, testing::_))
		.WillOnce(testing::DoAll(
				SetArgNPointeeTo<0>(correct_answer),
				testing::Return(correct_answer.length()))
				);

	EXPECT_EQ(aura_chip_uut.update_device_list(), test_devices.size());
	for (const auto& device : test_devices) {
		EXPECT_EQ(aura_chip_uut.get_device(device.first), device.second);
	}
}

TEST(aurachip_ut, reset_ok)
{
	const std::string correct_answer{
			"AT:START\r\n"
			"SOURCE:COMMAND\r\n"
			"COMMAND: RST\r\n"
			"STATUS:OK\r\n"
			"AT:STOP\r\n"
	};

	const std::string device_port{"COM6"};
	aura::chip aura_chip_uut{device_port};
	auto& serial_dev = aura_chip_uut.get_connection().get_serial_dev();
	TimeoutRAII timeout_raii;
	auto& timeout_instance = timeout_raii.get_instance();

	EXPECT_CALL(timeout_instance, sleep_for_ms(testing::_))
		.Times(1);

	EXPECT_CALL(serial_dev, flushReceiver())
		.Times(1);

	EXPECT_CALL(serial_dev, writeBytes(StrEqVoidPointer("AT+RST\n"), testing::_))
		.Times(1);

	EXPECT_CALL(serial_dev, readBytes(testing::_, testing::_, testing::_, testing::_))
		.WillOnce(testing::DoAll(
				SetArgNPointeeTo<0>(correct_answer),
				testing::Return(correct_answer.length()))
				);

	EXPECT_TRUE(aura_chip_uut.reset());
}

TEST(aurachip_ut, reset_fail)
{
	const std::string wrong_answer{
			"AT:START\r\n"
			"SOURCE:COMMAND\r\n"
			"COMMAND: RST\r\n"
			"STATUS:ERROR\r\n"
			"AT:STOP\r\n"
	};

	const std::string device_port{"COM6"};
	aura::chip aura_chip_uut{device_port};
	auto& serial_dev = aura_chip_uut.get_connection().get_serial_dev();
	TimeoutRAII timeout_raii;
	auto& timeout_instance = timeout_raii.get_instance();

	EXPECT_CALL(timeout_instance, sleep_for_ms(testing::_))
		.Times(1);

	EXPECT_CALL(serial_dev, flushReceiver())
		.Times(1);

	EXPECT_CALL(serial_dev, writeBytes(testing::_, testing::_))
		.Times(1);

	EXPECT_CALL(serial_dev, readBytes(testing::_, testing::_, testing::_, testing::_))
		.WillOnce(testing::DoAll(
				SetArgNPointeeTo<0>(wrong_answer),
				testing::Return(wrong_answer.length()))
				);

	EXPECT_FALSE(aura_chip_uut.reset());
}

TEST(aurachip_ut, set_xtal_correction_ok)
{
	const std::string correct_answer{
			"AT:START\r\n"
			"SOURCE:COMMAND\r\n"
			"COMMAND: CRYSTALCORRECTION\r\n"
			"STATUS:OK\r\n"
			"AT:STOP\r\n"
	};

	const std::string device_port{"COM6"};
	aura::chip aura_chip_uut{device_port};
	auto& serial_dev = aura_chip_uut.get_connection().get_serial_dev();
	TimeoutRAII timeout_raii;
	auto& timeout_instance = timeout_raii.get_instance();

	const int32_t xtal_value = -6548;
	const std::string xtal_cmd{"AT+CRYSTALCORRECTION=" + std::to_string(xtal_value) + "\n"};

	EXPECT_CALL(timeout_instance, sleep_for_ms(testing::_))
		.Times(1);

	EXPECT_CALL(serial_dev, flushReceiver())
		.Times(1);

	EXPECT_CALL(serial_dev, writeBytes(StrEqVoidPointer(xtal_cmd), testing::_))
		.Times(1);

	EXPECT_CALL(serial_dev, readBytes(testing::_, testing::_, testing::_, testing::_))
		.WillOnce(testing::DoAll(
				SetArgNPointeeTo<0>(correct_answer),
				testing::Return(correct_answer.length()))
				);

	EXPECT_TRUE(aura_chip_uut.set_xtal_correction(xtal_value));
}

TEST(aurachip_ut, set_xtal_correction_error)
{
	const std::string wrong_answer{
			"AT:START\r\n"
			"SOURCE:COMMAND\r\n"
			"COMMAND: CRYSTALCORRECTION\r\n"
			"STATUS:ERROR\r\n"
			"AT:STOP\r\n"
	};

	const std::string device_port{"COM6"};
	aura::chip aura_chip_uut{device_port};
	auto& serial_dev = aura_chip_uut.get_connection().get_serial_dev();
	TimeoutRAII timeout_raii;
	auto& timeout_instance = timeout_raii.get_instance();

	const int32_t xtal_value = -6548;

	EXPECT_CALL(timeout_instance, sleep_for_ms(testing::_))
		.Times(1);

	EXPECT_CALL(serial_dev, flushReceiver())
		.Times(1);

	EXPECT_CALL(serial_dev, writeBytes(testing::_, testing::_))
		.Times(1);

	EXPECT_CALL(serial_dev, readBytes(testing::_, testing::_, testing::_, testing::_))
		.WillOnce(testing::DoAll(
				SetArgNPointeeTo<0>(wrong_answer),
				testing::Return(wrong_answer.length()))
				);

	EXPECT_FALSE(aura_chip_uut.set_xtal_correction(xtal_value));
}

TEST(aurachip_ut, set_xtal_correction_fail)
{
	const std::string device_port{"COM6"};
	aura::chip aura_chip_uut{device_port};
	auto& serial_dev = aura_chip_uut.get_connection().get_serial_dev();
	TimeoutRAII timeout_raii;
	auto& timeout_instance = timeout_raii.get_instance();

	const std::string wrong_answer{"NULL\r\n"};
	const int32_t xtal_value = -6548;

	EXPECT_CALL(timeout_instance, sleep_for_ms(testing::_))
		.Times(1);

	EXPECT_CALL(serial_dev, flushReceiver())
		.Times(1);

	EXPECT_CALL(serial_dev, writeBytes(testing::_, testing::_))
		.Times(1);

	EXPECT_CALL(serial_dev, readBytes(testing::_, testing::_, testing::_, testing::_))
		.WillOnce(testing::DoAll(
				SetArgNPointeeTo<0>(wrong_answer),
				testing::Return(wrong_answer.length()))
				);

	EXPECT_FALSE(aura_chip_uut.set_xtal_correction(xtal_value));
}

TEST(aurachip_ut, get_xtal_correction_ok)
{
	const std::string device_port{"COM6"};
	aura::chip aura_chip_uut{device_port};
	auto& serial_dev = aura_chip_uut.get_connection().get_serial_dev();
	TimeoutRAII timeout_raii;
	auto& timeout_instance = timeout_raii.get_instance();

	const int32_t xtal_value = -6548;
	const std::string correct_answer{
			"AT:START\r\n"
			"SOURCE:COMMAND\r\n"
			"COMMAND: CRYSTALCORRECTION?\r\n"
			"STATUS:OK\r\n"
			"VALUE: " + std::to_string(xtal_value) + "\r\n"
			"AT:STOP\r\n"
	};

	EXPECT_CALL(timeout_instance, sleep_for_ms(testing::_))
		.Times(1);

	EXPECT_CALL(serial_dev, flushReceiver())
		.Times(1);

	EXPECT_CALL(serial_dev, writeBytes(StrEqVoidPointer("AT+CRYSTALCORRECTION?\n"), testing::_))
		.Times(1);

	EXPECT_CALL(serial_dev, readBytes(testing::_, testing::_, testing::_, testing::_))
		.WillOnce(testing::DoAll(
				SetArgNPointeeTo<0>(correct_answer),
				testing::Return(correct_answer.length()))
				);

	const auto read_value = aura_chip_uut.get_xtal_correction();
	EXPECT_TRUE(read_value);
	EXPECT_EQ(*read_value, xtal_value);
}

TEST(aurachip_ut, get_xtal_correction_failed_token)
{
	const std::string device_port{"COM6"};
	aura::chip aura_chip_uut{device_port};
	auto& serial_dev = aura_chip_uut.get_connection().get_serial_dev();
	TimeoutRAII timeout_raii;
	auto& timeout_instance = timeout_raii.get_instance();

	const int32_t xtal_value = -6548;
	const std::string wrong_answer{
			"AT:START\r\n"
			"SOURCE:COMMAND\r\n"
			"COMMAND: CRYSTALCORRECTION?\r\n"
			"STATUS:OK\r\n"
			"VAL: " + std::to_string(xtal_value) + "\r\n"
			"AT:STOP\r\n"
	};

	EXPECT_CALL(timeout_instance, sleep_for_ms(testing::_))
		.Times(1);

	EXPECT_CALL(serial_dev, flushReceiver())
		.Times(1);

	EXPECT_CALL(serial_dev, writeBytes(testing::_, testing::_))
		.Times(1);

	EXPECT_CALL(serial_dev, readBytes(testing::_, testing::_, testing::_, testing::_))
		.WillOnce(testing::DoAll(
				SetArgNPointeeTo<0>(wrong_answer),
				testing::Return(wrong_answer.length()))
				);

	const auto read_value = aura_chip_uut.get_xtal_correction();
	EXPECT_FALSE(read_value);
}

TEST(aurachip_ut, get_xtal_correction_failed_value)
{
	const std::string device_port{"COM6"};
	aura::chip aura_chip_uut{device_port};
	auto& serial_dev = aura_chip_uut.get_connection().get_serial_dev();
	TimeoutRAII timeout_raii;
	auto& timeout_instance = timeout_raii.get_instance();
	const std::string wrong_answer{
			"AT:START\r\n"
			"SOURCE:COMMAND\r\n"
			"COMMAND: CRYSTALCORRECTION?\r\n"
			"STATUS:OK\r\n"
			"VALUE: 123AA\r\n"
			"AT:STOP\r\n"
	};

	EXPECT_CALL(timeout_instance, sleep_for_ms(testing::_))
		.Times(1);

	EXPECT_CALL(serial_dev, flushReceiver())
		.Times(1);

	EXPECT_CALL(serial_dev, writeBytes(testing::_, testing::_))
		.Times(1);

	EXPECT_CALL(serial_dev, readBytes(testing::_, testing::_, testing::_, testing::_))
		.WillOnce(testing::DoAll(
				SetArgNPointeeTo<0>(wrong_answer),
				testing::Return(wrong_answer.length()))
				);

	const auto read_value = aura_chip_uut.get_xtal_correction();
	EXPECT_FALSE(read_value);
}
