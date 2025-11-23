#include "modian/core/engine/pinyin_engine.h"

#include <codecvt>
#include <fstream>
#include <sstream>

#include "modian/core/logger/logger_service.h"

namespace modian::inkstone::core {
	pinyin_engine::pinyin_engine() {
		try {
			const std::string dictionary_path = get_home_directory() + "/Modian/Local/pinyin_dictionary.txt";
			load_dictionary(dictionary_path);
		} catch (const std::exception& e) {
			logger_service::logger()->error(e.what());
		}
	}

	std::vector<std::wstring> pinyin_engine::convert(const std::wstring& input) {
		if (auto it = dictionary_.find(input); it != dictionary_.end()) {
			return it->second;
		}
		return {};
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