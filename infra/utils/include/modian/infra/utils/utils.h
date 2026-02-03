#pragma once

#include <string>
#include <string_view>

namespace modian::inkstone::infra::utils {
	std::wstring utf8_to_wstring(const std::string& str);
}