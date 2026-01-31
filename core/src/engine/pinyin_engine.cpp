#include "modian/core/engine/pinyin_engine.h"

#include <codecvt>
#include <fstream>
#include <sstream>

#include "modian/core/logger/logger_service.h"
#include "modian/core/utils/utils.h"

namespace modian::inkstone::core {
	pinyin_engine::pinyin_engine() {
		try {
			const std::string dictionary_path = get_home_directory() + "/Modian/Local/pinyin_dictionary.txt";
			load_dictionary(dictionary_path);
		} catch (const std::exception& e) {
			logger_service::logger()->error(e.what());
		}
	}

	void pinyin_engine::update_input_state(char c) {
		input_buffer_ += static_cast<wchar_t>(c);
		logger_service::logger()->debug("input buffer is: {}", utils::to_utf8(input_buffer_));
	}

	void pinyin_engine::handle_backspace() {
		if (!input_buffer_.empty()) {
			input_buffer_.pop_back();
		}
	}

	void pinyin_engine::reset() {
		input_buffer_.clear();
	}

	std::vector<std::wstring> pinyin_engine::get_candidates() const {
		if (input_buffer_.empty()) {
			return {};
		}

		if (auto it = dictionary_.find(input_buffer_); it != dictionary_.end()) {
			return it->second;
		}
		return {};
	}

	[[nodiscard]] std::string pinyin_engine::get_raw_input() const {
		return utils::to_utf8(input_buffer_);
	}

	void pinyin_engine::load_dictionary(const std::string& path) {
		std::wifstream file(path);
		file.imbue(std::locale(std::locale::classic(), new std::codecvt_utf8<wchar_t>));

		std::wstring line;
		while(std::getline(file, line)) {
			std::wistringstream iss(line);
			if (std::wstring pinyin; iss >> pinyin) {
				std::wstring word;
				auto& words = dictionary_[pinyin];
				while (iss >> word) {
					words.emplace_back(word);
				}
			}
		}
	}

	std::string pinyin_engine::get_home_directory() {
	#ifdef _WIN32
		if (const std::string userprofile = std::getenv("USERPROFILE"); !userprofile.empty()) {
			return userprofile;
		}
	#else
		if (const std::string home = std::getenv("HOME"); !home.empty()) {
			return home;
		}
	#endif
		throw std::runtime_error("Failed to retrieve home directory.");
	}
}
