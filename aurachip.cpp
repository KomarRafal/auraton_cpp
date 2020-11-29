/*
 * aurachip.cpp
 */
#include "aurachip.hpp"
#include "parser.hpp"

namespace aura
{

std::map<const int, const std::string> cmd::command_map = {
		{ cmd::TEST, "TEST" },
		{ cmd::VERSION, "VER" },
		{ cmd::ADDRESS, "ADDR" },
		{ cmd::DEV_LIST, "LIST" },
		{ cmd::LAST_CMD, "" }
};

bool chip::test() {
	const auto result = send_command(cmd::compose(cmd::TEST));
	return (result.compare("OK\r\n") == 0);
}

void chip::initialize() {
	auto device_str = send_command(cmd::compose(cmd::VERSION));
	device_str.append(send_command(cmd::compose(cmd::ADDRESS)));
	device dev(device_str);
	static_cast<device*>(this)->operator=(dev);
	initialize_flag = true;
}

int chip::update_device_list() {
#define EARLY_TEST
#ifdef EARLY_TEST
	const std::string dev_list_str = "ID: 1\r\n"
					"ADDRESS: 90FC4F9B\r\n"
					"PCODE: 300d\r\n"
					"FVER: 1.1\r\n"
					"HVER: 7\r\n"
					"MANCODE: 30\r\n"
					"ID: 2\r\n"
					"ADDRESS: 456223DA\r\n"
					"PCODE: 3005\r\n"
					"FVER: 1.5\r\n"
					"HVER: 1\r\n"
					"MANCODE: 30\r\n";
#else
	const auto dev_list_str = send_command(cmd::compose(cmd::DEV_LIST));
#endif
	const auto dev_list_parsed = parser::parse_device_list(dev_list_str);
	for (auto dev_pair : dev_list_parsed) {
		const device dev(dev_pair.second);
		device_list[dev_pair.first] = dev;
	}
	return device_list.size();
}

}
