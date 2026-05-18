#pragma once

#include <unordered_map>
#include<string>

namespace modian::inkstone::core {
    class dictionary {
    public:
        void build(const std::string& pinyin, std::string candidate);
        [[nodiscard]] std::vector<std::string> lookup(const std::string& input) const;
    private:
		std::unordered_map<std::string, std::vector<std::string>> data_;
    };
}
