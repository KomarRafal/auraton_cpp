/*
 * auraton.hpp
 */

#ifndef AURATON_HPP
#define AURATON_HPP

#include <string>
#include "serialib.h"

namespace aura
{
class auraton {
public:
	explicit auraton(const std::string& device) : device_port(device) { };
	bool open();
	void close();
	const std::string send_command(const std::string& command, int max_buffer_length = MAX_BUFFER_LENGTH);
	bool test_uart();

private:
	static const int MAX_BUFFER_LENGTH = 100;

	serialib serial_dev;
	const std::string device_port;
	const unsigned int baud_rate = 115200;
};

}

#endif /* AURATON_HPP */
