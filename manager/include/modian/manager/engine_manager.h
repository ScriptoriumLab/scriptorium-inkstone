#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <functional>
#include <utility>

#include "modian/manager/candidate_manager.h"
#include "modian/core/engine/input_engine.h"

namespace modian::inkstone::manager {
	using EngineFactory = std::function<std::shared_ptr<core::input_engine>()>;
	using EngineDetail = std::pair<std::string, EngineFactory>;

	class engine_manager {
	public:
		explicit engine_manager(std::shared_ptr<candidate_manager> can_manager);
		void add_new_engine(const EngineDetail& engine_detail);
		bool select_engine(const std::string& engine_name);
		void update_input_state(char character);
		void handle_backspace();
		void reset();
		[[nodiscard]] std::string get_current_raw_pinyin() const;

	private:
		std::unordered_map<std::string, EngineFactory> engine_factories_;
		std::shared_ptr<core::input_engine> current_engine_{nullptr};
		std::shared_ptr<candidate_manager> candidate_manager_;

		void _sync_candidates_from_engine();
	};
}