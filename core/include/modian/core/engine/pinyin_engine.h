#pragma once

#include "modian/core/engine/input_engine.h"
#include "modian/core/dictionary/dictionary.h"

#include <vector>

namespace modian::inkstone::core {
	class pinyin_engine : public input_engine {
	public:
		static constexpr std::string_view id{"pinyin engine"};

		explicit pinyin_engine(const std::string& dictionary_path);

		void update_input_state(char c) override;
		void handle_backspace() override;
		void reset() override;
		[[nodiscard]] std::vector<std::string> get_candidates() const override;
		[[nodiscard]] std::string get_raw_input() const override;

	private:
		void load_dictionary(const std::string& path);
		std::string input_buffer_;
		dictionary dictionary_;
	};
}
