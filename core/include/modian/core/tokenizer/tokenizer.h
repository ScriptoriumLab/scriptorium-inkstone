#pragma once

#include <string>
#include <unordered_set>
#include <vector>

namespace modian::inkstone::core {
    class tokenizer {
    public:
        std::vector<std::vector<std::string>> split(std::string_view raw_string);

    private:
        static constexpr int MAX_SPELLING_LEN{6};
        std::unordered_set<std::string_view> syllabary{
            "ni", "hao", "shi", "jie",
            "fan", "gan", "fang", "an"
        };
    };
}
