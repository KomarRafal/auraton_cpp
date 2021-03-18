/*
 * parser.hpp
 */

#ifndef PARSER_HPP_
#define PARSER_HPP_

#include <cstdint>
#include <string>
#include <map>

#include "parameter.hpp"

namespace aura
{

class parser_legacy {
public:
	typedef std::map<int32_t, std::string> device_list_t;
	static const std::string OK_TOKEN;
	static const std::string DEVICE_TOKEN;
	static const std::string CODE_TOKEN;
	static const std::string CHANNEL_TOKEN;
	static const std::string FLAG_OWN_TOKEN;
	static const std::string FLAG_WRITEABLE_TOKEN;
	static const std::string VALUE_TOKEN;
	static const std::string EOL;
	static std::string parse(const std::string& input_str, const std::string& token, bool omit_token = true);
	static device_list_t parse_device_list(const std::string& input_str);
	static bool get_next_parameter(std::string& input_string, parameter& read_parameter);
	static bool check_result(const std::string& input_str);
};

}
#endif /* PARSER_HPP_ */
