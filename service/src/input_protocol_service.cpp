#include "modian/service/input_protocol_service.h"

namespace modian::inkstone::service {
	core::protocol::input::v1::key_event input_protocol_service::parse_key_event_request(const std::string& request) {
		return core::protocol::input::v1::key_event{request};
	}

	std::string input_protocol_service::build_instruction_response(const core::protocol::input::v1::instruction& instruction) {
		return instruction.encode();
	}
}
