#include "scriptorium/orchestrator/session_orchestrator.h"

namespace scriptorium::inkstone::manager {
	session_orchestrator::session_orchestrator(std::unique_ptr<candidate_manager> candidate_manager, std::unique_ptr<engine_manager> engine_manager)
		: candidate_manager_{std::move(candidate_manager)}, engine_manager_{std::move(engine_manager)} {}

    felt::core::protocol::input::v1::instruction session_orchestrator::handle_key(const felt::core::protocol::input::v1::key_event& key_event) {
        const std::string& key = key_event.content;

        if (key.empty()) return {
            felt::core::protocol::input::v1::message_type::UPDATE,
            { "", {} }
        };

        auto candidates = engine_manager_->get_current_candidates();
        bool has_candidates = !candidates.empty();

        if (key == "cmd:left") {
            if (has_candidates && highlight_index_ > 0) {
                --highlight_index_;
                auto current_path = candidates[highlight_index_].spelling_path;
                update_ui(std::move(candidates));
                return { felt::core::protocol::input::v1::message_type::UPDATE, { engine_manager_->get_current_raw_input(), current_path } };
            }
            auto current_path = has_candidates ? candidates[highlight_index_].spelling_path : std::vector<std::string>{};
            return { felt::core::protocol::input::v1::message_type::UPDATE, { engine_manager_->get_current_raw_input(), current_path } };
        }

        if (key == "cmd:right") {
            if (has_candidates && highlight_index_ < candidates.size() - 1) {
                ++highlight_index_;
                auto current_path = candidates[highlight_index_].spelling_path;
                update_ui(std::move(candidates));
                return { felt::core::protocol::input::v1::message_type::UPDATE, { engine_manager_->get_current_raw_input(), current_path } };
            }
            auto current_path = has_candidates ? candidates[highlight_index_].spelling_path : std::vector<std::string>{};
            return { felt::core::protocol::input::v1::message_type::UPDATE, { engine_manager_->get_current_raw_input(), current_path } };
        }

        if (key == "cmd:space") {
            if (has_candidates) {
                const auto candidate = candidates[highlight_index_];
                engine_manager_->reset();
                highlight_index_ = 0;
                update_ui({});
                return {
                    felt::core::protocol::input::v1::message_type::COMMIT,
                    { candidate.word, candidate.spelling_path }
                };
            }
            return { felt::core::protocol::input::v1::message_type::COMMIT, { " ", {} } };
        }

        if (key == "cmd:backspace") {
            engine_manager_->handle_backspace();
        } else if (key.size() == 1) {
            engine_manager_->update_input_state(key[0]);
            highlight_index_ = 0;
        }

        candidates = engine_manager_->get_current_candidates();
        has_candidates = !candidates.empty();

        auto current_path = has_candidates ? candidates[highlight_index_].spelling_path : std::vector<std::string>{};
        update_ui(std::move(candidates));

        return {
            felt::core::protocol::input::v1::message_type::UPDATE,
            { engine_manager_->get_current_raw_input(), current_path }
        };
    }

	void session_orchestrator::update_ui(std::vector<core::candidate> candidates) const {
		candidate_manager_->update_state(candidates, highlight_index_);
	}

    core::candidate session_orchestrator::select_candidate(size_t index) const {
		return engine_manager_->get_current_candidates()[index];
	}
}
