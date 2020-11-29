/*
 * parser.hpp
 */

#ifndef PARSER_HPP_
#define PARSER_HPP_

#include <string>
#include <map>

namespace aura
{

class parser {
public:
	typedef std::map<int, std::string> device_list_t;
	static const std::string OK_TOKEN;
	static const std::string DEVICE_TOKEN;
	static const std::string EOL;
	static std::string parse(std::string input_str, std::string token, bool omit_token = true);
	static device_list_t parse_device_list(std::string input_str);
	static bool check_result(const std::string& input_str);
};

}
#endif /* PARSER_HPP_ */
