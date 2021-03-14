/*
 * source.hpp
 */

#ifndef SOURCE_HPP_
#define SOURCE_HPP_

#include "simple_token.hpp"

namespace aura
{

namespace parser
{

enum source_type {
	COMMAND,
	EVENT,
};

class source : public simple_token<source_type> {
public:
	virtual const std::string& get_token() const;
	virtual const token_map& get_token_map() const;

private:
	const std::string SOURCE_TOKEN = "SOURCE:";
	const token_map source_map = {
			{ "COMMAND", source_type::COMMAND },
			{ "EVENT", source_type::EVENT },
	};
};

}

}

#endif
