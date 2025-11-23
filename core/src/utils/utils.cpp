#include "modian/core/utils/utils.h"

#if defined(_WIN32)
#include <windows.h>

namespace modian::inkstone::core::utils {
	std::string to_utf8(std::wstring_view wstr) {
		if (wstr.empty()) return {};

		const int size_needed = WideCharToMultiByte(
			CP_UTF8, 0,
			wstr.data(), static_cast<int>(wstr.size()),
			nullptr, 0, nullptr, nullptr
		);

		if (size_needed <= 0) return {};

		std::string str(size_needed, 0);
		WideCharToMultiByte(
			CP_UTF8, 0,
			wstr.data(), static_cast<int>(wstr.size()),
			str.data(), size_needed, nullptr, nullptr
		);

		return str;
	}
}

#endif
