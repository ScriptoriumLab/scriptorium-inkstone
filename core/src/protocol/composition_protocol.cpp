#include <utility>

#include "modian/core/protocol/composition_protocol.h"

namespace modian::inkstone::core::protocol {
	composition_protocol::composition_protocol() : type{message_type::NONE} {}
	composition_protocol::composition_protocol(const message_type& type, std::string  payload) : type{type}, payload{std::move(payload)} {}

	std::string composition_protocol::encode() const {
		if (type == message_type::NONE) return "";
		if (payload.empty()) return "";

		std::string draft_message;
		draft_message.reserve(2 + payload.size());
		draft_message += static_cast<char>(type);
		draft_message += ':';
		draft_message += payload;

		return draft_message;
	}

	composition_protocol composition_protocol::create_update(std::string payload) {
		return {message_type::UPDATE, std::move(payload)};
	}

	composition_protocol composition_protocol::create_commit(std::string payload) {
		return {message_type::COMMIT, std::move(payload)};
	}
}
