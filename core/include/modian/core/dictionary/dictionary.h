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

		trie_node data_;
    };
}
