#pragma once

#include <array>
#include <vector>
#include <memory>
#include<string>

namespace modian::inkstone::core {
    class dictionary {
    public:
        void build(const std::string& pinyin, std::string candidate);
        [[nodiscard]] std::vector<std::string> lookup(const std::string& input) const;
    private:
        struct trie_node {
            std::vector<std::string> candidates;
            std::array<std::unique_ptr<trie_node>, 26> sub_node;
        };

        void get_candidates(const trie_node* node, std::vector<std::string>& candidates) const;

        const int MAX_CANDIDATES{50};
		trie_node data_;
    };
}
