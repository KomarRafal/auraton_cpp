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

enum status_type {
	OK,
	ERROR,
	_NULL,
	FULL,
	IN_PROGRESS,
	NOT_ACCEPTED
};

class status : public simple_token<status_type> {
public:
	virtual const std::string& get_token() const;
	virtual const token_map& get_token_map() const;

private:
	const std::string STATUS_TOKEN = "STATUS:";
	const token_map status_map = {
			{ "OK", status_type::OK },
			{ "ERROR", status_type::ERROR },
			{ "NULL", status_type::_NULL },
			{ "FULL", status_type::FULL },
			{ "IN_PROGRESS", status_type::IN_PROGRESS },
			{ "NOT_ACCEPTED", status_type::NOT_ACCEPTED },
	};
};

}

}

#endif
