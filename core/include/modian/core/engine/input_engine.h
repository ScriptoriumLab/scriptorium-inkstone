#pragma once

#include <vector>
#include <string>
#include <functional>
#include <utility>
#include <memory>

namespace modian::inkstone::core {
	class input_engine {
  	public:
		virtual ~input_engine() = default;

		virtual std::vector<std::wstring> convert(const std::wstring& input) = 0;
	};

	template<typename T>
	std::pair<std::string, std::function<std::shared_ptr<input_engine>()>> lazy_load_dictionary() {
		return std::make_pair(std::string{T::id}, []() -> std::shared_ptr<input_engine> { return std::make_shared<T>(); });
	}
}