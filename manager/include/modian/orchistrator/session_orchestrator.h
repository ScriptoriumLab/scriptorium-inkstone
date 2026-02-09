#pragma once

#include "modian/core/protocol/instruction.h"
#include "modian/core/protocol/key_event.h"
#include "modian/manager/candidate_manager.h"
#include "modian/manager/engine_manager.h"

namespace modian::inkstone::manager {
	class session_orchestrator {
	public:
		session_orchestrator(std::shared_ptr<candidate_manager> candidate_manager, std::shared_ptr<engine_manager> engine_manager);
		[[nodiscard]] core::protocol::input::v1::instruction handle_key(const core::protocol::input::v1::key_event& key_event) const;
		[[nodiscard]] std::string select_candidate(const size_t& index) const;

	private:
		std::shared_ptr<candidate_manager> candidate_manager_;
		std::shared_ptr<engine_manager> engine_manager_;
	};
}
