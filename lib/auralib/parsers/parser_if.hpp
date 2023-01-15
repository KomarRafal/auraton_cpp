/*
 * parser_if.hpp
 */

#ifndef PARSER_IF_HPP_
#define PARSER_IF_HPP_

#include <string_view>
#include <optional>
#include <string>
#include <memory>
#include <vector>

namespace aura
{

namespace parser
{

class parser_if {
public:
	using parser_ptr_t = std::unique_ptr<parser_if>;
	using parser_algorithm_t = std::vector<parser_ptr_t>;
	using parsed_value_t = std::optional<std::string>;

	virtual parsed_value_t parse(std::string_view& message) = 0;
	const std::string EOL = "\r\n";
	const std::string OK = "OK";

	virtual ~parser_if() { };
};

}

}

#endif
