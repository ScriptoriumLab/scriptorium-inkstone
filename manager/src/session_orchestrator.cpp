#include "modian/orchistrator/session_orchestrator.h"

namespace modian::inkstone::manager {
	session_orchestrator::session_orchestrator(std::unique_ptr<candidate_manager> candidate_manager, std::unique_ptr<engine_manager> engine_manager)
		: candidate_manager_{std::move(candidate_manager)}, engine_manager_{std::move(engine_manager)} {}

	core::protocol::input::v1::instruction session_orchestrator::handle_key(const core::protocol::input::v1::key_event& key_event) {
		const std::string& key = key_event.content;

        if (key.empty()) return {core::protocol::input::v1::message_type::UPDATE, ""};

        auto candidates = engine_manager_->get_current_candidates();
        bool has_candidates = !candidates.empty();

        if (key == "cmd:left") {
            if (has_candidates && highlight_index_ > 0) {
                highlight_index_--;
                update_ui(candidates);
            }
            return {core::protocol::input::v1::message_type::UPDATE, engine_manager_->get_current_raw_pinyin()};
        }

        if (key == "cmd:right") {
            if (has_candidates && highlight_index_ < candidates.size() - 1) {
                highlight_index_++;
                update_ui(candidates);
            }
            return {core::protocol::input::v1::message_type::UPDATE, engine_manager_->get_current_raw_pinyin()};
        }

        if (key == "cmd:space") {
            if (has_candidates) {
                std::string text = candidates[highlight_index_];
                engine_manager_->reset();
                candidate_manager_->update_state({}, 0);
                highlight_index_ = 0;
                return {core::protocol::input::v1::message_type::COMMIT, text};
            }
            return {core::protocol::input::v1::message_type::COMMIT, " "};
        }

        if (key == "cmd:backspace") {
            engine_manager_->handle_backspace();
        } else if (key.size() == 1) {
            engine_manager_->update_input_state(key[0]);
            highlight_index_ = 0;
        }
		candidates = engine_manager_->get_current_candidates();
		candidate_manager_->update_state(candidates, highlight_index_);

		std::string raw_pinyin = engine_manager_->get_current_raw_pinyin();

		return {core::protocol::input::v1::message_type::UPDATE, std::move(raw_pinyin)};
	}

	void session_orchestrator::update_ui(const std::vector<std::string>& candidates) const {
		candidate_manager_->update_state(candidates, highlight_index_);
	}

	std::string session_orchestrator::select_candidate(const size_t& index) const {
		return engine_manager_->get_current_candidates()[index];
	}
}
