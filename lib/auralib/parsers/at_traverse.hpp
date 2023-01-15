/*
 * at_traverse.hpp
 */

#ifndef AT_TRAVERSE_HPP_
#define AT_TRAVERSE_HPP_

#include <string_view>
#include "parser_if.hpp"

namespace aura
{

namespace parser
{

class at_traverse {
public:
	explicit at_traverse(std::string_view &message) :
		message(message) {

	}
	parser_if::parsed_value_t get_next_block();
private:
	std::string_view &message;
	const std::string BEGIN_TOKEN = "AT:START\r\n";
	const std::string END_TOKEN = "AT:STOP\r\n";
};


}

}

#endif
