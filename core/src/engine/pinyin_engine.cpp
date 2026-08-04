#include "scriptorium/core/engine/pinyin_engine.h"

#include <fstream>
#include <sstream>

#include "scriptorium/felt/core/logger/logger_service.h"

namespace scriptorium::inkstone::core {
	pinyin_engine::pinyin_engine(const std::string& dictionary_path) {
		try {
			load_dictionary(dictionary_path);
		} catch (const std::exception& e) {
            felt::core::logger_service::logger()->error(e.what());
		}
	}

	void pinyin_engine::update_input_state(const char c) {
		input_buffer_ += c;
        felt::core::logger_service::logger()->debug("input buffer is: {}", input_buffer_);
	}

	void pinyin_engine::handle_backspace() {
		if (!input_buffer_.empty()) {
			input_buffer_.pop_back();
		}
	}

	void pinyin_engine::reset() {
		input_buffer_.clear();
	}

    namespace {
        void backtrace_assemble_sentences(const std::vector<std::vector<std::string>>& candidate_matrix, int starter, std::string& sentence, std::vector<std::string>& sentences) {
            if (starter == candidate_matrix.size()) {
                sentences.push_back(sentence);
                return;
            }

            for (const auto& word : candidate_matrix[starter]) {
                auto old_length = sentence.size();
                sentence.append(word);

                backtrace_assemble_sentences(candidate_matrix, starter + 1, sentence, sentences);

                sentence.resize(old_length);
            }
        }
    }

	std::vector<std::string> pinyin_engine::get_candidates() const {
		if (input_buffer_.empty()) {
			return {};
		}

        auto spelling_paths = tokenizer_.split(input_buffer_);
        if (spelling_paths.empty()) {
            return {};
        }

        std::vector<std::string> final_candidates;
        for (const auto& path : spelling_paths) {
            std::vector<std::vector<std::string>> candidate_matrix;
            for (const auto& spelling : path) {
                candidate_matrix.push_back(dictionary_.lookup(spelling));
            }

            std::vector<std::string> sentences;
            std::string sentence;
            backtrace_assemble_sentences(candidate_matrix, 0, sentence, sentences);

            final_candidates.insert(final_candidates.end(), sentences.begin(), sentences.end());
        }

        return final_candidates;
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
                tokenizer_.add_syllabary(entry.spelling);
			}
		}
	}
}
