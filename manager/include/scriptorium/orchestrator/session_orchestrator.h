#pragma once

#include "scriptorium/felt/core/protocol/v1/input/instruction.h"
#include "scriptorium/felt/core/protocol/v1/input/key_event.h"
#include "scriptorium/manager/candidate_manager.h"
#include "scriptorium/manager/engine_manager.h"

namespace scriptorium::inkstone::manager {
	class session_orchestrator {
	public:
		session_orchestrator(std::unique_ptr<candidate_manager> candidate_manager, std::unique_ptr<engine_manager> engine_manager);
		[[nodiscard]] felt::core::protocol::input::v1::instruction handle_key(const felt::core::protocol::input::v1::key_event& key_event);
		[[nodiscard]] std::string select_candidate(size_t index) const;

	private:
		size_t highlight_index_{0};
		std::unique_ptr<candidate_manager> candidate_manager_;
		std::unique_ptr<engine_manager> engine_manager_;

		void update_ui(std::vector<std::string> candidates) const;
	};
}
