/*
 * auraton.hpp
 */

#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <string>
#include "serialib.h"

namespace aura
{
class connection {
public:
	explicit connection(const std::string& device) : device_port{device} { };
	bool open();
	void close();
	const std::string send_command(const std::string& command, int max_buffer_length = MAX_BUFFER_LENGTH);
	bool simple_command(const std::string& command);
	bool test_uart();

private:
	static const int MAX_BUFFER_LENGTH = 200;

	serialib serial_dev;
	const std::string device_port;
	const unsigned int baud_rate = 115200;
};

}

#endif /* CONNECTION_HPP */
