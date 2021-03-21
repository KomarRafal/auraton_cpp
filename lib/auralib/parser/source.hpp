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

class source : public simple_token {
public:
	virtual const std::string& get_token() const;

private:
	const std::string SOURCE_TOKEN = "SOURCE:";
};

}

}

#endif
