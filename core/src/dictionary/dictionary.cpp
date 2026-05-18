#include "modian/core/dictionary/dictionary.h"

namespace modian::inkstone::core {
    void dictionary::build(const std::string& pinyin, std::string candidate) {
        data_[pinyin].push_back(std::move(candidate));
    }

    std::vector<std::string> dictionary::lookup(const std::string& input) const {
		if (const auto it = data_.find(input); it != data_.end()) {
			return it->second;
		}
		return {};
    }
}
