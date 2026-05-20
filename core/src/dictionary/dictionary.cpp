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
        if (node == nullptr) return;

        for (const auto& candidate : node->candidates) {
            candidates.push_back(candidate);
            std::push_heap(candidates.begin(), candidates.end(), cmp);

            if (candidates.size() > MAX_CANDIDATES) {
                std::pop_heap(candidates.begin(), candidates.end(), cmp);
                candidates.pop_back();
            }
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
        candidates.reserve(MAX_CANDIDATES + 1);
        get_candidates(curr, candidates);
        std::sort_heap(candidates.begin(), candidates.end(), cmp);

        return candidates
            | std::views::transform([](const auto& entry) { return entry.word; })
            | std::ranges::to<std::vector>();
    }
}
