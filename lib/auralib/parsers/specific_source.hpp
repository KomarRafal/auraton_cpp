/*
 * specific_source.hpp
 */

#ifndef SPECIFIC_SOURCE_HPP_
#define SPECIFIC_SOURCE_HPP_

#include <string>
#include "source.hpp"

namespace aura
{

namespace parser
{

class specific_source : public source {
public:
	explicit specific_source(std::string specific_source);
	virtual bool is_value_ok(const std::string_view& value) const;

private:
	std::string source;
};

}

}

#endif
