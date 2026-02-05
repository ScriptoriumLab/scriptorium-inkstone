#pragma once

#include <string>

namespace modian::inkstone::infra::utils {
	std::wstring utf8_to_wstring(std::string_view str);
	std::string get_dict_path();
}