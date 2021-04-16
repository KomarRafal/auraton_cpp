/*
 * builder_if.hpp
 */

#ifndef BUILDER_IF_HPP_
#define BUILDER_IF_HPP_

#include <memory>
#include <vector>

#include "parsers/parser_if.hpp"

namespace aura
{

namespace parser
{

namespace commands
{

class builder_if {
public:
	using parser_ptr = std::unique_ptr<parser_if>;
	using builder_t = std::vector<parser_ptr>;
	virtual builder_t build() const = 0;
};

}

}

}

#endif
