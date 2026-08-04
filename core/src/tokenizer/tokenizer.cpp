#include "scriptorium/core/tokenizer/tokenizer.h"

namespace scriptorium::inkstone::core {
    namespace {
        void dfs(std::string_view raw_string, const std::vector<std::vector<int>>& dag, int i, std::vector<std::string>& current_path, std::vector<std::vector<std::string>>& tokens) {
            if (i == raw_string.size()) {
                tokens.push_back(current_path);
                return;
            }

            for (int next_idx : dag[i]) {
                current_path.push_back(std::string{raw_string.substr(i, next_idx - i)});
                dfs(raw_string, dag, next_idx, current_path, tokens);
                current_path.pop_back();
            }
        }
    }

    std::vector<std::vector<std::string>> tokenizer::split(std::string_view raw_string) const {
        const int N = raw_string.size();

        std::vector<std::vector<int>> possible_path_dag(N, {});
        for (int l = 0; l < N; ++l) {
            for (int r = l + 1; r <= std::min(N, l + MAX_SPELLING_LEN); ++r) {
                auto sub_str = std::string{raw_string.substr(l, r - l)};
                if (syllabary.count(sub_str) > 0) {
                    possible_path_dag[l].push_back(r);
                }
            }
            if (possible_path_dag[l].empty()) {
                possible_path_dag[l].push_back(N);
            }
        }

        std::vector<std::vector<std::string>> tokens;
        std::vector<std::string> current_path;
        if (N > 0) {
            dfs(raw_string, possible_path_dag, 0, current_path, tokens);
        }

        return tokens;
    }

    void tokenizer::add_syllabary(std::string spelling) {
        syllabary.insert(std::move(spelling));
    }
}
