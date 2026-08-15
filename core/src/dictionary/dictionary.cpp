#include "scriptorium/core/dictionary/dictionary.h"

#include <algorithm>
#include <ranges>

namespace scriptorium::inkstone::core {
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
        curr->dictionary_entries.push_back(entry);
    }

    void dictionary::get_candidate_dictionary_entries(const trie_node* node, std::vector<dictionary_entry>& candidate_dictionary_entries) const {
        if (node == nullptr) return;

        for (const auto& candidate : node->dictionary_entries) {
            candidate_dictionary_entries.push_back(candidate);
            std::push_heap(candidate_dictionary_entries.begin(), candidate_dictionary_entries.end(), cmp);

            if (candidate_dictionary_entries.size() > MAX_CANDIDATES) {
                std::pop_heap(candidate_dictionary_entries.begin(), candidate_dictionary_entries.end(), cmp);
                candidate_dictionary_entries.pop_back();
            }
        }

        for (const auto& sub : node->sub_node) {
            get_candidate_dictionary_entries(sub.get(), candidate_dictionary_entries);
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

        std::vector<dictionary_entry> candidate_dictionary_entries;
        candidate_dictionary_entries.reserve(MAX_CANDIDATES + 1);
        get_candidate_dictionary_entries(curr, candidate_dictionary_entries);
        std::sort_heap(candidate_dictionary_entries.begin(), candidate_dictionary_entries.end(), cmp);

        return candidate_dictionary_entries
            | std::views::transform([](const auto& entry) { return entry.word; })
            | std::ranges::to<std::vector>();
    }
}
