#pragma once

#include <string>
#include <string_view>

namespace modian::inkstone::core::utils {
	std::string to_utf8(std::wstring_view wstr);
}