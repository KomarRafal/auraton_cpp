/*
 * connection.cpp
 */

#include "connection.hpp"

namespace aura

{

bool connection::open() {
	const char status = serial_dev.openDevice(device_port.c_str(), baud_rate);
	return (status == 1);
}

void connection::close() {
	serial_dev.closeDevice();
}

const std::string connection::send_command(const std::string &command, int max_buffer_length) {
	char input_buffer[max_buffer_length + 1] = { };
	serial_dev.flushReceiver();
	serial_dev.writeBytes(command.c_str(), command.length());
	serial_dev.readBytes(input_buffer, sizeof(input_buffer), 100, 0);
	return std::string(input_buffer);
}

bool connection::test_uart() {
	const std::string result = send_command("AT\n", 5);
	return (result.compare("OK\r\n") == 0);
}

}
