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

		void update_input_state(wchar_t character);

		bool handle_backspace();
		void reset();
	private:
		std::unordered_map<std::string, EngineFactory> engine_factories_;
		std::shared_ptr<core::input_engine> current_engine_{nullptr};

		std::wstring input_pinyin_;
		std::shared_ptr<candidate_manager> candidate_manager_;

		void _trigger_conversion();
	};
}