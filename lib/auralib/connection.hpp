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
	const std::string send_command(const std::string& command, uint32_t max_buffer_length = MAX_BUFFER_LENGTH);
	bool simple_command(const std::string& command);
	bool check_event(const std::string &event);
	bool test_uart();

private:
	static const uint32_t MAX_BUFFER_LENGTH = 200;
	static const uint32_t WAIT_SLEEP_MS = 100;
	serialib serial_dev;
	const std::string device_port;
	const uint32_t baud_rate = 115200;
};

}

#endif /* CONNECTION_HPP */