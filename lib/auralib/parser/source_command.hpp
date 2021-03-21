/*
 * source_command.hpp
 */

#ifndef SOURCE_COMMAND_HPP_
#define SOURCE_COMMAND_HPP_

#include "source.hpp"

namespace aura
{

namespace parser
{

class source_command : public source {
public:
	virtual bool is_value_ok(const std::string_view& value) const;
};

}

}

#endif
