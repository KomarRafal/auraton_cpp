/*
 * connection.cpp
 */
#include <timeout.hpp>
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
	for (auto iter = 0U; iter < iterations; iter++) {
		if (serial_dev.available() > 0) {
			return true;
		}
		Timeout::sleep_for_ms(WAIT_SLEEP_MS);
	}
	if (serial_dev.available() > 0) {
		return true;
	}
	else if (last_wait == 0) {
		return false;
	}
	else {
		Timeout::sleep_for_ms(last_wait);
	}

	return (serial_dev.available() > 0);
}

void connection::clear_receiver() {
	const std::string flush_sequence{"\r\n"};
	serial_dev.writeBytes(flush_sequence.c_str(), flush_sequence.length());
	Timeout::sleep_for_ms(50);
	serial_dev.flushReceiver();
}

const std::string connection::send_command(const std::string& command, uint16_t max_buffer_length, uint32_t wait_time_ms) {
	std::unique_ptr<char[]> input_buffer = std::make_unique<char[]>(max_buffer_length + 1);
	serial_dev.flushReceiver();
	serial_dev.writeBytes(command.c_str(), command.length());
	if (wait_time_ms > 0) {
		Timeout::sleep_for_ms(wait_time_ms);
	}
	const auto bytes_count = serial_dev.readBytes(input_buffer.get(), max_buffer_length, 100, 0);
	if (bytes_count <= 0) {
		return std::string();
	}
	return std::string(input_buffer.get());
}

// TODO: can be removed?
bool connection::simple_command(const std::string& command, uint32_t wait_time_ms) {
	const std::string result = send_command(command, MAX_BUFFER_LENGTH, wait_time_ms);
	return parser_legacy::check_result(result);
}

// TODO: can be removed?
bool connection::check_event(const std::string& event) {
	const uint32_t buffer_length = event.length() + 10;
	std::unique_ptr<char[]> input_buffer = std::make_unique<char[]>(buffer_length);
	const auto bytes_count = serial_dev.readBytes(input_buffer.get(), buffer_length, 100, 0);
	if (bytes_count <= 0) {
		return false;
	}
	const auto read_event = parser_legacy::parse(input_buffer.get(), event, false);
	if (read_event.length() != event.length()) {
		return false;
	}
	return parser_legacy::check_result(std::string(input_buffer.get() + event.length() + parser_legacy::EOL.length()));
}

// TODO: check only AT layer?
bool connection::test_uart() {
	return simple_command("AT\n\r");
}

}
