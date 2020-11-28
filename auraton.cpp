/*
 * auraton.cpp
 */

#include "auraton.hpp"

namespace aura

{

bool auraton::open() {
	const char status = serial_dev.openDevice(device_port.c_str(), baud_rate);
	return (status == 1);
}

void auraton::close() {
	serial_dev.closeDevice();
}

const std::string auraton::send_command(const std::string &command, int max_buffer_length) {
	if (max_buffer_length > MAX_BUFFER_LENGTH)
		max_buffer_length = MAX_BUFFER_LENGTH;
	char input_buffer[max_buffer_length + 1] = { };
	serial_dev.flushReceiver();
	serial_dev.writeBytes(command.c_str(), command.length());
	serial_dev.readBytes(input_buffer, sizeof(input_buffer), 100, 0);
	return std::string(input_buffer);
}

bool auraton::test_uart() {
	const std::string result = send_command("AT\n", 5);
	return (result.compare("OK\r\n") == 0);
}

}
