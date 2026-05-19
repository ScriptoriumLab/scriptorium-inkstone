#include "modian/core/dictionary/dictionary.h"

#include <algorithm>
#include <ranges>

namespace modian::inkstone::core {
    void dictionary::build(const dictionary_entry& entry) {
        auto curr = &data_;
        for (const auto& c : entry.spelling) {
            if (c < 'a' || c > 'z') {
                continue;
            }
            const auto idx = c - 'a';
            if (!curr->sub_node[idx]) {
                curr->sub_node[idx] = std::make_unique<trie_node>();
            }
            curr = curr->sub_node[idx].get();
        }
        curr->candidates.push_back(entry);
    }

    void dictionary::get_candidates(const trie_node* node, std::vector<dictionary_entry>& candidates) const {
        if (node == nullptr || candidates.size() == MAX_CANDIDATES) return;

        for (const auto& candidate : node->candidates) {
            if (candidates.size() == MAX_CANDIDATES) break;
            candidates.push_back(candidate);
        }

        for (const auto& sub : node->sub_node) {
            get_candidates(sub.get(), candidates);
        }
    }

    std::vector<std::string> dictionary::lookup(const std::string& spelling) const {
        auto curr = &data_;
        for (const auto& c : spelling) {
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

        std::vector<dictionary_entry> candidates;
        candidates.reserve(MAX_CANDIDATES);
        get_candidates(curr, candidates);
        std::sort(candidates.begin(), candidates.end(), [](const auto& l, const auto& r){
            return l.weight > r.weight;
        });

        return candidates
            | std::views::transform([](const auto& entry) { return entry.word; })
            | std::ranges::to<std::vector>();
    }
}
