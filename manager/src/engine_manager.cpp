#include "modian/manager/engine_manager.h"
#include "modian/core/logger/logger_service.h"

namespace modian::inkstone::manager {

    engine_manager::engine_manager(std::shared_ptr<candidate_manager> can_manager)
        : candidate_manager_{std::move(can_manager)} {}

    void engine_manager::add_new_engine(const EngineDetail& engine_detail) {
        engine_factories_.emplace(engine_detail.first, engine_detail.second);

        if (!current_engine_) {
            select_engine(engine_detail.first);
        }
    }

    bool engine_manager::select_engine(const std::string& engine_name) {
        auto it = engine_factories_.find(engine_name);
        if (it != engine_factories_.end()) {
            current_engine_ = it->second();
            core::logger_service::logger()->info("Switched to engine: {}", engine_name);
            reset();
            return true;
        }
        return false;
    }

    void engine_manager::update_input_state(char character) {
        if (!current_engine_) return;

        char lower_char = std::tolower(static_cast<unsigned char>(character));
        current_engine_->update_input_state(lower_char);
        _sync_candidates_from_engine();
    }

    void engine_manager::handle_backspace() {
        if (!current_engine_) return;

        current_engine_->handle_backspace();
        _sync_candidates_from_engine();
    }

    void engine_manager::reset() {
        if (current_engine_) {
            current_engine_->reset();
        }

        candidate_manager_->update_candidates({});
    }

    std::string engine_manager::get_current_raw_pinyin() const {
        if (current_engine_) {
            return current_engine_->get_raw_input();
        }
        return "";
    }

    void engine_manager::_sync_candidates_from_engine() {
        if (!current_engine_) return;

        auto candidates = current_engine_->get_candidates();
        candidate_manager_->update_candidates(std::move(candidates));
    }
}