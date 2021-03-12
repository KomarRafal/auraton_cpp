/*
 * source.hpp
 */

#ifndef SOURCE_HPP_
#define SOURCE_HPP_

#include <string>
#include <map>
#include "parser_if.hpp"

namespace aura
{

namespace parser
{

enum source_type {
	COMMAND,
	EVENT,
};

class source : public parser_if<source_type> {
public:
	virtual std::optional<source_type> parse(std::string_view& message);

private:
	const std::string SOURCE_TOKEN = "SOURCE:";
	const std::map<std::string, source_type> source_map = {
			{ "COMMAND", source_type::COMMAND },
			{ "EVENT", source_type::EVENT },
	};
};

}

}

#endif
