#pragma once

#include <string>

namespace modian::inkstone::core::protocol::input::v1 {
	enum class message_type {
		NONE = 'N',
		UPDATE = 'U',
		COMMIT = 'C'
	};

	struct instruction {
		message_type type{message_type::NONE};
		std::string payload{};
	};
}
