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

		virtual void update_input_state(char c) = 0;
		virtual void handle_backspace() = 0;
		virtual void reset() = 0;
		virtual std::vector<std::wstring> get_candidates() const = 0;
		virtual std::string get_raw_input() const = 0;
	};

	template<typename T>
	std::pair<std::string, std::function<std::shared_ptr<input_engine>()>> lazy_load_dictionary() {
		return std::make_pair(std::string{T::id}, []() -> std::shared_ptr<input_engine> { return std::make_shared<T>(); });
	}
}