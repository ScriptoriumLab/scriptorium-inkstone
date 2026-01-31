#pragma once

#include "modian/core/engine/input_engine.h"

#include <unordered_map>
#include <vector>

namespace modian::inkstone::core {
	class pinyin_engine : public input_engine {
	public:
		static constexpr std::string_view id{"pinyin engine"};

		pinyin_engine();

		void update_input_state(char c) override;
		void handle_backspace() override;
		void reset() override;
		std::vector<std::wstring> get_candidates() const override;
		std::string get_raw_input() const override;

		void load_dictionary(const std::string& path);
	private:
		static std::string get_home_directory();
		std::wstring input_buffer_;
		std::unordered_map<std::wstring, std::vector<std::wstring>> dictionary_;
	};
}