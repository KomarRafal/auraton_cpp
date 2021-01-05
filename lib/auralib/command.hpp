/*
 * command.hpp
 */

#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <cstdint>
#include <string>
#include <map>

namespace aura {

class command {
public:
	enum code {
		TEST,
		VERSION,
		LINK,
		UNLINK,
		ADDRESS,
		DEV_LIST,
		FACTORY_RESET,
		RESET,
		EVENT_LINK,
		GET_DEV_OPTION,
		GET_XTAL_CORRECTION,
		SET_XTAL_CORRECTION,
		LAST_CMD
	};

	static const std::string& Get(code command_code)
	{
		if (command_code > LAST_CMD)
			command_code = LAST_CMD;
		return command_map[command_code];
	}

	static const std::string compose(const code& command_code, const std::string& attributes = {});

private:
	static std::map<const code, const std::string> command_map;
};

}
#endif /* COMMAND_HPP */
