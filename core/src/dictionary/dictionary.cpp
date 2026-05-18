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

    void dictionary::get_candidates(const trie_node* node, std::vector<std::string>& candidates) const {
        if (node == nullptr || candidates.size() == MAX_CANDIDATES) return;

        for (const auto& candidate : node->candidates) {
            if (candidates.size() == MAX_CANDIDATES) break;
            candidates.push_back(candidate);
        }

        for (const auto& sub : node->sub_node) {
            get_candidates(sub.get(), candidates);
        }
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

        std::vector<std::string> candidates;
        candidates.reserve(MAX_CANDIDATES);
        get_candidates(curr, candidates);

        return candidates;
    }
}
