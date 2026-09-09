#include "scriptorium/orchestrator/session_orchestrator.h"

namespace scriptorium::inkstone::manager {
	session_orchestrator::session_orchestrator(std::unique_ptr<candidate_manager> candidate_manager, std::unique_ptr<engine_manager> engine_manager)
		: candidate_manager_{std::move(candidate_manager)}, engine_manager_{std::move(engine_manager)} {}

    felt::core::protocol::input::v1::instruction session_orchestrator::handle_key(const felt::core::protocol::input::v1::key_event& key_event) {
        using namespace felt::core::protocol::input::v1;

        auto candidates = engine_manager_->get_current_candidates();
        const bool has_candidates = !candidates.empty();

        switch (key_event.type) {
        case key_event_type::LEFT: {
            if (has_candidates && highlight_index_ > 0) {
                --highlight_index_;
                const auto current_path = candidates[highlight_index_].spelling_path;

                update_ui(std::move(candidates));

                return {
                    message_type::UPDATE,
                    {engine_manager_->get_current_raw_input(), current_path}
                };
            }

            const auto current_path =
                has_candidates
                    ? candidates[highlight_index_].spelling_path
                    : std::vector<std::string>{};

            return {
                message_type::UPDATE,
                {engine_manager_->get_current_raw_input(), current_path}
            };
        }

        case key_event_type::RIGHT: {
            if (has_candidates && highlight_index_ < candidates.size() - 1) {
                ++highlight_index_;
                const auto current_path = candidates[highlight_index_].spelling_path;

                update_ui(std::move(candidates));

                return {
                    message_type::UPDATE,
                    {engine_manager_->get_current_raw_input(), current_path}
                };
            }

            const auto current_path =
                has_candidates
                    ? candidates[highlight_index_].spelling_path
                    : std::vector<std::string>{};

            return {
                message_type::UPDATE,
                {engine_manager_->get_current_raw_input(), current_path}
            };
        }

        case key_event_type::SPACE: {
            if (has_candidates) {
                const auto candidate = candidates[highlight_index_];

                engine_manager_->reset();
                highlight_index_ = 0;
                update_ui({});

                return {
                    message_type::COMMIT,
                    {candidate.word, candidate.spelling_path}
                };
            }

            return {
                message_type::COMMIT,
                {" ", {}}
            };
        }

        case key_event_type::BACKSPACE:
            engine_manager_->handle_backspace();
            break;

        case key_event_type::TEXT: {
            if (!key_event.content.has_value() || key_event.content->empty()) {
                return {
                    message_type::UPDATE,
                    {"", {}}
                };
            }

            engine_manager_->update_input_state(key_event.content->front());
            highlight_index_ = 0;
            break;
        }
        }

        candidates = engine_manager_->get_current_candidates();

        const bool has_updated_candidates = !candidates.empty();

        const auto current_path =
            has_updated_candidates
                ? candidates[highlight_index_].spelling_path
                : std::vector<std::string>{};

        update_ui(std::move(candidates));

        return {
            message_type::UPDATE,
            {engine_manager_->get_current_raw_input(), current_path}
        };
    }

	void session_orchestrator::update_ui(std::vector<core::candidate> candidates) const {
		candidate_manager_->update_state(candidates, highlight_index_);
	}

    core::candidate session_orchestrator::select_candidate(size_t index) const {
		return engine_manager_->get_current_candidates()[index];
	}
}
