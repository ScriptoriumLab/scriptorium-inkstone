#pragma once

#include <string>
#include <vector>

namespace scriptorium::inkstone::core {
    struct candidate {
        std::string word;
        std::vector<std::string> spelling_path;
    };
}
