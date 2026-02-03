#pragma once

#include <string>

namespace modian::inkstone::infra::utils {
	std::wstring utf8_to_wstring(const std::string& str);
	std::string get_dict_path();
}