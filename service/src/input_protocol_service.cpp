#include "modian/service/input_protocol_service.h"

namespace modian::inkstone::service {
    common::core::protocol::input::v1::key_event input_protocol_service::parse_key_event_request(const std::string& request) {
		return common::core::protocol::input::v1::key_event{request};
	}

	std::string input_protocol_service::build_instruction_response(const common::core::protocol::input::v1::instruction& instruction) {
		if (instruction.type == common::core::protocol::input::v1::message_type::NONE) return "";
		if (instruction.payload.empty()) return "";

		std::string draft_message;
		draft_message.reserve(2 + instruction.payload.size());
		draft_message += static_cast<char>(instruction.type);
		draft_message += ':';
		draft_message += instruction.payload;

		return draft_message;
	}
}
