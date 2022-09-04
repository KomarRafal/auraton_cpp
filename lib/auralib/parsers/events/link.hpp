/*
 * link.hpp
 */

#ifndef EVENT_LINK_HPP_
#define EVENT_LINK_HPP_

#include "parser_if.hpp"

/*
AT:START
SOURCE:EVENT
EVENT:LINK
STATUS:OK
DIRECTION:RECEIVE
ID: 1
ADDRESS: 30090005
PCODE: 3009
FVER: 1.11
HVER: 1.0
MANCODE: 30
CODE: 32
CHANNEL: 0
FLAG OWN: 0
FLAG WRITEABLE: 1
VALUE: 0
(...)
AT:STOP
*/
namespace aura
{

namespace parser
{

namespace events
{

class link : public parser_if
{
public:
	static const uint32_t LINK_WAIT_MS = 32000;
	static const uint32_t MAX_BUFFER_BYTES = 3000;
	virtual parsed_value parse(std::string_view& message);
	static const std::string get_event_token();
private:
	static const std::string EVENT_LINK_TOKEN;
};

class link_builder
{
public:
	static parser_if::parser_algorithm_t build();
};

}

}

}

#endif
