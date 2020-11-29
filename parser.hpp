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
	static const std::string DEVICE_TOKEN;
	static const std::string EOL;
	static std::string parse(std::string input_str, std::string token);
	static device_list_t parse_device_list(std::string input_str);
};

}
#endif /* PARSER_HPP_ */
