#include "modian/infra/utils/utils.h"

#if defined(_WIN32)
#include <windows.h>

namespace modian::inkstone::infra::utils {
	std::wstring utf8_to_wstring(const std::string& str) {
		if (str.empty()) return {};
		int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), static_cast<int>(str.size()), nullptr, 0);
		std::wstring wstrTo(size_needed, 0);
		MultiByteToWideChar(CP_UTF8, 0, str.c_str(), static_cast<int>(str.size()), &wstrTo[0], size_needed);
		return wstrTo;
	}
}

#endif
