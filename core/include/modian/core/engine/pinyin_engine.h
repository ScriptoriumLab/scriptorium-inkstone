#pragma once

#include "modian/core/engine/input_engine.h"

#include <unordered_map>
#include <vector>

namespace modian::inkstone::core {
	class pinyin_engine : public input_engine {
	public:
		static constexpr std::string_view id{"pinyin engine"};

		pinyin_engine();
		std::vector<std::wstring> convert(const std::wstring& input) override;

		void load_dictionary(const std::string& path);
	private:
		static std::string get_home_directory();
		std::unordered_map<std::wstring, std::vector<std::wstring>> dictionary_;
	};
}