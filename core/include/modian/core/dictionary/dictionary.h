#pragma once

#include <array>
#include <vector>
#include <memory>
#include <string>
#include <istream>

namespace modian::inkstone::core {
    struct dictionary_entry {
        std::string spelling;
        std::string word;
        int weight{};

        friend std::istream& operator>>(std::istream& is, dictionary_entry& entry) {
            is >> entry.spelling >> entry.word >> entry.weight;
            return is;
        }
    };

    class dictionary {
    public:
        void build(const dictionary_entry& entry);
        [[nodiscard]] std::vector<std::string> lookup(const std::string& spelling) const;
    private:
        struct trie_node {
            std::vector<dictionary_entry> candidates;
            std::array<std::unique_ptr<trie_node>, 26> sub_node;
        };

        void get_candidates(const trie_node* node, std::vector<dictionary_entry>& candidates) const;

        const int MAX_CANDIDATES{50};
		trie_node data_;
    };
}
