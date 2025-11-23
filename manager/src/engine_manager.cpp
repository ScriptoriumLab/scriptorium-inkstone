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
            return true;
        }
        return false;
    }

    void engine_manager::update_input_state(wchar_t character) {
        if (!current_engine_) return;

        input_pinyin_.push_back(character);

        _trigger_conversion();
    }

    bool engine_manager::handle_backspace() {
        if (!input_pinyin_.empty()) {
            input_pinyin_.pop_back();
            _trigger_conversion();
            return true;
        }

        reset();
        return false;
    }

    void engine_manager::reset() {
        input_pinyin_.clear();
        candidate_manager_->update_candidates({});
    }

    void engine_manager::_trigger_conversion() {
        if (input_pinyin_.empty()) {
            candidate_manager_->update_candidates({});
            return;
        }

        auto candidates = current_engine_->convert(input_pinyin_);

        candidate_manager_->update_candidates(std::move(candidates));
    }
}