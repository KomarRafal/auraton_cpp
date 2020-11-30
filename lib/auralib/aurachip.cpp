/*
 * aurachip.cpp
 */
#include "aurachip.hpp"
#include "parser.hpp"

namespace aura
{

std::map<const uint32_t, const std::string> cmd::command_map = {
		{ cmd::TEST, "TEST?" },
		{ cmd::VERSION, "VER?" },
		{ cmd::LINK, "LINK" },
		{ cmd::EVENT_LINK, "EVENT_LINK" },
		{ cmd::ADDRESS, "ADDR?" },
		{ cmd::DEV_LIST, "LIST?" },
		{ cmd::RESET, "RST" },
		{ cmd::LAST_CMD, "" }
};

const std::string cmd::compose(cmd::code command) {
	std::string composed_command("AT+");
	composed_command.append(cmd::Get(command));
	composed_command.append("\n");
	return composed_command;
}

bool chip::test() {
	return simple_command(cmd::compose(cmd::TEST));
}

void chip::initialize() {
	auto device_str = send_command(cmd::compose(cmd::VERSION));
	device_str.append(send_command(cmd::compose(cmd::ADDRESS)));
	device dev(device_str);
	static_cast<device*>(this)->operator=(dev);
	initialize_flag = true;
}

bool chip::link() {
	const bool is_cmd_ok = simple_command(cmd::compose(cmd::LINK));
	if (!is_cmd_ok)
		return false;
	const bool is_event = wait_for_read(LINK_WAIT_MS);
	if (!is_event)
		return false;
	const bool is_event_ok = check_event(cmd::Get(cmd::EVENT_LINK));
	return is_event_ok;
}

bool chip::reset() {
	return simple_command(cmd::compose(cmd::RESET));
}

int chip::update_device_list() {
//#define EARLY_TEST
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
	device_list.clear();
	for (auto dev_pair : dev_list_parsed) {
		const device dev(dev_pair.second);
		device_list[dev_pair.first] = dev;
	}
	return device_list.size();
}

}
