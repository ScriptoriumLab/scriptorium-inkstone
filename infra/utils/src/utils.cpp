#include "modian/infra/utils/utils.h"

#include <filesystem>
#include <cstdlib> // for std::getenv
#ifdef _WIN32
#include <windows.h>
#endif

namespace modian::inkstone::infra::utils {
	std::wstring utf8_to_wstring(const std::string& str) {
		if (str.empty()) return {};
		int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), static_cast<int>(str.size()), nullptr, 0);
		std::wstring wstrTo(size_needed, 0);
		MultiByteToWideChar(CP_UTF8, 0, str.c_str(), static_cast<int>(str.size()), &wstrTo[0], size_needed);
		return wstrTo;
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
