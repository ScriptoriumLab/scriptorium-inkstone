#pragma once

#include "modian/common/core/protocol/v1/input/instruction.h"
#include "modian/common/core/protocol/v1/input/key_event.h"

namespace modian::inkstone::service {
	class input_protocol_service {
	public:
		static common::core::protocol::input::v1::key_event parse_key_event_request(const std::string& request);
		static std::string build_instruction_response(const common::core::protocol::input::v1::instruction& instruction);
	};
}
