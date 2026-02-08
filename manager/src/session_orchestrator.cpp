#include "modian/orchistrator/session_orchestrator.h"

#include "modian/core/logger/logger_service.h"

namespace modian::inkstone::manager {
	session_orchestrator::session_orchestrator(std::shared_ptr<candidate_manager> candidate_manager, std::shared_ptr<engine_manager> engine_manager)
		: candidate_manager_{std::move(candidate_manager)}, engine_manager_{std::move(engine_manager)} {}

	core::protocol::input::v1::instruction session_orchestrator::handle_key(const core::protocol::input::v1::key_event& key_event) const {
		if (key_event.content.empty()) {
			return {core::protocol::input::v1::message_type::UPDATE, ""};
		}

		if (const auto key = key_event.content[0]; key == '\b') {
			core::logger_service::logger()->info("[Recv] CMD: Backspace");
			engine_manager_->handle_backspace();
		} else {
			core::logger_service::logger()->info("[Recv] Key: {}", key);
			engine_manager_->update_input_state(key);
		}

		if (const auto candidates = engine_manager_->get_current_candidates(); !candidates.empty()) {
			std::string text = candidates[0];
			engine_manager_->reset();
			candidate_manager_->update_candidates({});
			core::logger_service::logger()->info("Decision: Commit '{}'", text);

			return {core::protocol::input::v1::message_type::COMMIT, std::move(text)};
		}

		std::string raw_pinyin = engine_manager_->get_current_raw_pinyin();
		core::logger_service::logger()->info("Decision: Update '{}'", raw_pinyin);

		return {core::protocol::input::v1::message_type::UPDATE, std::move(raw_pinyin)};
	}
}
