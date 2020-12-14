/*
 * parser.cpp
 */

#include "parser.hpp"

namespace aura
{

const std::string parser::OK_TOKEN = "OK";
const std::string parser::DEVICE_TOKEN = "ID: ";
const std::string parser::EOL = "\r\n";
const std::string parser::VALUE_TOKEN = "VALUE: ";

std::string parser::parse(const std::string& input_str, const std::string& token, bool omit_token) {
	auto begin = input_str.find(token);
	if (begin == std::string::npos)
		return "";
	if (omit_token)
		begin += token.length();
	const auto end = input_str.find(EOL, begin);
	return input_str.substr(begin, end - begin);
}

parser::device_list_t parser::parse_device_list(const std::string& input_str) {
	device_list_t device_list;
	size_t begin = 0;

	while (true)
	{
		size_t end;
		begin = input_str.find(DEVICE_TOKEN, begin);
		if (begin == std::string::npos)
			return device_list;
		end = input_str.find(EOL, begin);
		begin += DEVICE_TOKEN.length();
		auto dev_id_str = input_str.substr(begin, end - begin);
		if (dev_id_str.empty())
			return device_list;
		auto dev_id = std::stoul(dev_id_str);
		begin = end + EOL.length();
		end = input_str.find(DEVICE_TOKEN, begin);
		if (device_list.find(dev_id) == device_list.end())
			device_list[dev_id] = input_str.substr(begin, end - begin);
		else
			device_list.find(dev_id)->second.append(input_str.substr(begin, end - begin));

		begin = end;
	}

}

bool parser::check_result(const std::string& input_str) {
	const std::string status = parse(input_str, OK_TOKEN, false);
	return (status.length() == OK_TOKEN.length());
}
}
