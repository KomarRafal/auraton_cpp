/*
 * auraton.hpp
 */

#ifndef CONNECTION_HPP
#define CONNECTION_HPP
#include <cstdint>
#include <string>
#include "serialib.h"

namespace aura
{
class connection {
public:
	explicit connection(const std::string& device) : device_port{device} { };
	bool open();
	void close();
	bool wait_for_read(uint32_t max_time_ms = WAIT_SLEEP_MS);
	void clear_receiver();
	const std::string send_command(const std::string& command, uint16_t max_buffer_length = MAX_BUFFER_LENGTH, uint32_t wait_time_ms = WAIT_TIMEOUT_MS);
	const std::string read_event(uint16_t max_buffer_length, uint32_t max_wait_time_ms);
	serialib& get_serial_dev() {
		return serial_dev;
	}

private:
	serialib serial_dev;
	static const uint16_t MAX_BUFFER_LENGTH = 300;
	static const uint32_t WAIT_SLEEP_MS = 100;
	static const uint32_t WAIT_TIMEOUT_MS = 10;
	const std::string device_port;
	const uint32_t baud_rate = 115200;
};

}

#endif /* CONNECTION_HPP */
