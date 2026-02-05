#include "modian/infra/utils/utils.h"

#include <filesystem>
#include <cstdlib> // for std::getenv
#ifdef _WIN32
#include <windows.h>
#endif

namespace modian::inkstone::infra::utils {
	std::wstring utf8_to_wstring(std::string_view str) {
		return std::filesystem::path(str).c_str();
	}

	std::string get_dict_path() {
		std::string home;
#ifdef _WIN32
		if (auto p = std::getenv("USERPROFILE")) home = p;
#else
		if (auto p = std::getenv("HOME")) home = p;
#endif
		if (home.empty()) {
			home = ".";
		}

		auto path = std::filesystem::path(home) / "Modian" / "Local" / "pinyin_dictionary.txt";
		return path.string();
	}
}
