#include "scriptorium/manager/engine_manager.h"

#include "scriptorium/felt/core/logger/logger_service.h"

namespace scriptorium::inkstone::manager {
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
            felt::core::logger_service::logger()->info("Switched to engine: {}", engine_name);
            reset();
            return true;
        }
        return false;
    }

    void engine_manager::update_input_state(const char character) const {
        if (!current_engine_) return;

        const char lower_char = std::tolower(static_cast<unsigned char>(character));
        current_engine_->update_input_state(lower_char);
    }

    void engine_manager::handle_backspace() const {
        if (!current_engine_) return;

        current_engine_->handle_backspace();
    }

    void engine_manager::reset() const {
        if (current_engine_) {
            current_engine_->reset();
        }
    }

    std::vector<std::string> engine_manager::get_current_candidates() const {
        return current_engine_->get_candidates();
    }

    std::string engine_manager::get_current_raw_pinyin() const {
        if (current_engine_) {
            return current_engine_->get_raw_input();
        }
        return "";
    }
}
