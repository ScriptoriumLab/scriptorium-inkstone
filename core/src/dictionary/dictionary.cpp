#include "modian/core/dictionary/dictionary.h"

namespace modian::inkstone::core {
    void dictionary::build(const std::string& pinyin, std::string candidate) {
        auto curr = &data_;
        for (const auto& c : pinyin) {
            if (c < 'a' || c > 'z') {
                continue;
            }
            const auto idx = c - 'a';
            if (!curr->sub_node[idx]) {
                curr->sub_node[idx] = std::make_unique<trie_node>();
            }
            curr = curr->sub_node[idx].get();
        }
        curr->candidates.push_back(std::move(candidate));
    }

    std::vector<std::string> dictionary::lookup(const std::string& input) const {
        auto curr = &data_;
        for (const auto& c : input) {
            if (c < 'a' || c > 'z') {
                continue;
            }
            const auto idx = c - 'a';
            if (!curr->sub_node[idx]) {
                return {};
            } else {
                curr = curr->sub_node[idx].get();
            }
        }

        return curr->candidates;
    }
}
