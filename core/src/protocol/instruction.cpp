#include <utility>

#include "modian/core/protocol/instruction.h"

namespace modian::inkstone::core::protocol::input::v1 {
	instruction::instruction() : type{message_type::NONE} {}
	instruction::instruction(const message_type& type, std::string  payload) : type{type}, payload{std::move(payload)} {}

	std::string instruction::encode() const {
		if (type == message_type::NONE) return "";
		if (payload.empty()) return "";

		std::string draft_message;
		draft_message.reserve(2 + payload.size());
		draft_message += static_cast<char>(type);
		draft_message += ':';
		draft_message += payload;

		return draft_message;
	}

	instruction instruction::create_update(std::string payload) {
		return {message_type::UPDATE, std::move(payload)};
	}

	instruction instruction::create_commit(std::string payload) {
		return {message_type::COMMIT, std::move(payload)};
	}
}
