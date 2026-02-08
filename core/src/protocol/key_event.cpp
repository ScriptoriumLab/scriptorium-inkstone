#include "modian/core/protocol/key_event.h"

#include <utility>

namespace modian::inkstone::core::protocol::input::v1 {
	key_event::key_event(std::string content) : content{std::move(content)} {}
}
