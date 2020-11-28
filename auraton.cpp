#include <iostream>
#include <stdlib.h>
#include <vector>
#include <string>
#include <map>

#include "serialib.h"

class auraton {
public:
	explicit auraton(const std::string& device) : device_port(device) { }
	bool open() {
		const char status = serial_dev.openDevice(device_port.c_str(), baud_rate);
		return (status == 1);
	}

	void close() {
		serial_dev.closeDevice();
	}

	const std::string send_command(const std::string& command, int max_buffer_length = MAX_BUFFER_LENGTH)
	{
		if (max_buffer_length > MAX_BUFFER_LENGTH)
			max_buffer_length = MAX_BUFFER_LENGTH;
		char input_buffer[max_buffer_length + 1] = { };
		serial_dev.flushReceiver();
		serial_dev.writeBytes(command.c_str(), command.length());
		serial_dev.readBytes(input_buffer, sizeof(input_buffer), 100, 0);
		return std::string(input_buffer);
	}

	bool test_uart() {
		const std::string result = send_command("AT\n", 5);
		return (result.compare("OK\r\n") == 0);
	}

private:
	static const int MAX_BUFFER_LENGTH = 100;

	serialib serial_dev;
	const std::string device_port;
	const unsigned int baud_rate = 115200;
};

class cmd {
public:
	enum {
		TEST,
		VERSION,
		LAST_CMD
	};

	static const std::string& Get(int command)
	{
		if (command > LAST_CMD)
			command = LAST_CMD;
		return command_map[command];
	}

private:
	static std::map<const int, const std::string> command_map;
};

std::map<const int, const std::string> cmd::command_map = {
		{ cmd::TEST, "TEST" },
		{ cmd::VERSION, "VER"},
		{ cmd::LAST_CMD, ""}
};

namespace aura {
class version {
public:
	explicit version(std::string version_str) :
		product_code(),
		fw_version(),
		hw_version(),
		manufacture_code()
	{
		std::map<std::string, std::string*> version_info = {
				{"PCODE: ", &product_code},
				{"FVER: ", &fw_version},
				{"HVER: ", &hw_version},
				{"MANCODE: ", &manufacture_code}
			};

		for (auto info : version_info)
		{
			auto begin = version_str.find(info.first);
			if (begin == std::string::npos)
				continue;
			begin += info.first.length();
			const auto end = version_str.find("\r\n", begin);
			info.second->append(version_str.substr(begin, end - begin));
		}
	}

	std::string get_product_code() const
	{
		return product_code;
	}

	std::string get_fw_version() const
	{
		return fw_version;
	}

	std::string get_hw_version() const
	{
		return hw_version;
	}

	std::string get_manufacture_code() const
	{
		return manufacture_code;
	}

private:
	std::string product_code;
	std::string fw_version;
	std::string hw_version;
	std::string manufacture_code;
};

class chip :
	public auraton {
public:

	explicit chip(const std::string& device) : auraton(device) { }

	bool test()
	{
		const auto result = send_command(compose_command(cmd::TEST));
		return (result.compare("OK\r\n") == 0);
	}

	std::string get_version()
	{
		const auto version = send_command(compose_command(cmd::VERSION));
		return version;
	}

private:
	const std::string compose_command(int command)
	{
		std::string composed_command("AT+");
		composed_command.append(cmd::Get(command));
		composed_command.append("?\n");
		return composed_command;
	}
};

}

int main(int argc, char **argv) {
	aura::chip my_aura_chip("COM8");
	const bool ret = my_aura_chip.open();
	std::cout << "status: " << std::boolalpha << ret << std::endl;
	std::cout << "test_uart: " << my_aura_chip.test_uart() << std::endl;;
	std::cout << "test: " << my_aura_chip.test() << std::endl;
	const std::string version = my_aura_chip.get_version();
	std::cout << "Version:\n" << version << std::endl;
	const aura::version aura_ver(version);
	std::cout << "product_code: " << aura_ver.get_product_code() << std::endl;
	std::cout << "fw_version: " << aura_ver.get_fw_version() << std::endl;
	std::cout << "hw_version: " << aura_ver.get_hw_version() << std::endl;
	std::cout << "manufacture_code: " << aura_ver.get_manufacture_code() << std::endl;
	my_aura_chip.close();
	return 0;
}
