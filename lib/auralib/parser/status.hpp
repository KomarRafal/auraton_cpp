/*
 * status.hpp
 */

#ifndef STATUS_HPP_
#define STATUS_HPP_

#include "simple_token.hpp"

namespace aura
{

namespace parser
{

class status : public simple_token {
public:
	virtual const std::string& get_token() const;

private:
	const std::string STATUS_TOKEN = "STATUS:";
};

}

}

#endif
