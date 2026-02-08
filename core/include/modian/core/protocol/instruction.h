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

		std::string encode() const;

		static instruction create_update(std::string payload);
		static instruction create_commit(std::string payload);

	private:
		message_type type;
		std::string payload;
	};
}