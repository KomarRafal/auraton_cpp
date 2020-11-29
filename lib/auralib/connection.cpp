/*
 * connection.cpp
 */
#include <chrono>
#include <thread>

#include "connection.hpp"
#include "parser.hpp"

namespace aura

{

bool connection::open() {
	const char status = serial_dev.openDevice(device_port.c_str(), baud_rate);
	return (status == 1);
}

void connection::close() {
	serial_dev.closeDevice();
}

bool connection::wait_for_read(uint32_t max_time_ms) {
	const uint32_t iterations =  max_time_ms / WAIT_SLEEP_MS;
	const uint32_t last_wait = max_time_ms % WAIT_SLEEP_MS;
	for (auto iter = 0; iter < iterations; iter++) {
		if (serial_dev.available() > 0)
			return true;
		std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int64_t>(WAIT_SLEEP_MS)));
	}
	if (serial_dev.available() > 0)
		return true;
	else if (last_wait == 0)
		return false;
	else
		std::this_thread::sleep_for(std::chrono::milliseconds(last_wait));
	return (serial_dev.available() > 0);
}

const std::string connection::send_command(const std::string &command, uint32_t max_buffer_length) {
	char input_buffer[max_buffer_length + 1] = { };
	serial_dev.flushReceiver();
	serial_dev.writeBytes(command.c_str(), command.length());
	serial_dev.readBytes(input_buffer, sizeof(input_buffer), 100, 0);
	return std::string(input_buffer);
}

bool connection::simple_command(const std::string& command) {
	const std::string result = send_command(command);
	return parser::check_result(result);
}

bool connection::check_event(const std::string &event) {
	char input_buffer[event.length() + 10] = { };
	serial_dev.readBytes(input_buffer, sizeof(input_buffer), 100, 0);
	const auto read_event = parser::parse(input_buffer, event, false);
	if (read_event.length() != event.length())
		return false;
	return parser::check_result(std::string(input_buffer + event.length() + parser::EOL.length()));
}

bool connection::test_uart() {
	return simple_command("AT\n\r");
}

}
