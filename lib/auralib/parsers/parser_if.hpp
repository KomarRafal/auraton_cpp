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
	using parser_ptr = std::unique_ptr<parser_if>;
	using parser_algorithm_t = std::vector<parser_ptr>;
	using parsed_value = std::optional<std::string>;

	virtual parsed_value parse(std::string_view& message) = 0;
	const std::string EOL = "\r\n";
	const std::string OK = "OK";

	// TODO: move it to some parser_base just after parser_if and add some UT
	parsed_value find(const std::string_view& message, const std::string& token) {
		const auto begin = message.find(token);
		if (begin == std::string::npos) {
			return std::nullopt;
		}
		const auto end = message.find(EOL, begin);
		if (end == std::string::npos) {
			return  static_cast<std::string>(message.substr(begin));
		}
		return static_cast<std::string>(message.substr(begin, end - begin));
	}
	virtual ~parser_if() { };
};

}

}

#endif
