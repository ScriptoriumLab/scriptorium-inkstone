#pragma once

#include <string>

namespace modian::inkstone::core::protocol::input::v1 {
	enum class message_type {
		NONE = 'N',
		UPDATE = 'U',
		COMMIT = 'C'
	};

	class instruction {
	public:
		instruction();
		instruction(const message_type& type, std::string payload);

		message_type type;
		std::string payload;
	};
}