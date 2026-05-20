#pragma once

#include <string>
#include <unordered_set>
#include <vector>

namespace modian::inkstone::core {
    class tokenizer {
    public:
        std::vector<std::vector<std::string>> split(std::string_view raw_string) const;
        void add_syllabary(std::string spelling);

    private:
        static constexpr int MAX_SPELLING_LEN{6};
        std::unordered_set<std::string> syllabary{};
    };
}
