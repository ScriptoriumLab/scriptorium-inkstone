#pragma once

#include <concepts>
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
		[[nodiscard]] virtual std::vector<std::string> get_candidates() const = 0;
		[[nodiscard]] virtual std::string get_raw_input() const = 0;
	};

    template<typename T>
    concept engine_plugin = std::derived_from<T, input_engine> && requires {
         { T::id } -> std::convertible_to<std::string_view>;
     };

	template<engine_plugin T>
	std::pair<std::string, std::function<std::shared_ptr<input_engine>()>> lazy_load_dictionary(const std::string& dictionary_path) {
		return std::make_pair(std::string{T::id}, [dictionary_path]() -> std::shared_ptr<input_engine> { return std::make_shared<T>(dictionary_path); });
	}
}
