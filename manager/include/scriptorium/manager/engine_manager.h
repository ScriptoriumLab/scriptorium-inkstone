#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <functional>
#include <utility>

#include "scriptorium/core/engine/input_engine.h"

namespace scriptorium::inkstone::manager {
	using EngineFactory = std::function<std::shared_ptr<core::input_engine>()>;
	using EngineDetail = std::pair<std::string, EngineFactory>;

	class engine_manager {
	public:
		void add_new_engine(const EngineDetail& engine_detail);
		bool select_engine(const std::string& engine_name);
		void update_input_state(char character) const;
		void handle_backspace() const;
		void reset() const;
		[[nodiscard]] std::vector<core::candidate> get_current_candidates() const;
		[[nodiscard]] std::string get_current_raw_pinyin() const;

	private:
		std::unordered_map<std::string, EngineFactory> engine_factories_;
		std::shared_ptr<core::input_engine> current_engine_{nullptr};
	};
}
