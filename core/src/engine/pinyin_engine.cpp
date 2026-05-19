#include "modian/core/engine/pinyin_engine.h"

#include <fstream>
#include <sstream>

#include "modian/common/core/logger/logger_service.h"

namespace modian::inkstone::core {
	pinyin_engine::pinyin_engine(const std::string& dictionary_path) {
		try {
			load_dictionary(dictionary_path);
		} catch (const std::exception& e) {
            common::core::logger_service::logger()->error(e.what());
		}
	}

	void pinyin_engine::update_input_state(const char c) {
		input_buffer_ += c;
        common::core::logger_service::logger()->debug("input buffer is: {}", input_buffer_);
	}

	void pinyin_engine::handle_backspace() {
		if (!input_buffer_.empty()) {
			input_buffer_.pop_back();
		}
	}

	void pinyin_engine::reset() {
		input_buffer_.clear();
	}

	std::vector<std::string> pinyin_engine::get_candidates() const {
		if (input_buffer_.empty()) {
			return {};
		}

        return dictionary_.lookup(input_buffer_);
	}

	[[nodiscard]] std::string pinyin_engine::get_raw_input() const {
		return input_buffer_;
	}

	void pinyin_engine::load_dictionary(const std::string& path) {
		std::ifstream file(path);
		if (!file.is_open()) {
			throw std::runtime_error("Cannot open dictionary file at: " + path);
		}

		std::string line;
		while(std::getline(file, line)) {
			if (line.empty()) continue;

			std::istringstream iss(line);
			if (dictionary_entry entry; iss >> entry) {
                dictionary_.build(entry);
			}
		}
	}
}
