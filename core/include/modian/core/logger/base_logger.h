#pragma once

#include <string>
#include <string_view>
#include <format>
#include <type_traits>

namespace modian::inkstone::core {

    enum class log_level { debug, info, error };

    class base_logger {
    public:
        virtual ~base_logger() = default;
        virtual void sink_it(log_level level, std::string_view msg) = 0;
        [[nodiscard]] virtual std::string_view type() const { return "base_logger"; }

        template<typename... Args>
        requires (sizeof...(Args) > 0)
        void info(std::format_string<std::type_identity_t<Args>...> fmt, Args&&... args) {
            sink_formatted(log_level::info, fmt.get(), std::make_format_args(args...));
        }

        void info(std::string_view msg)  { sink_it(log_level::info, msg); }

        template<typename... Args>
        requires (sizeof...(Args) > 0)
        void debug(std::format_string<Args...> fmt, Args&&... args) {
            sink_formatted(log_level::debug, fmt.get(), std::make_format_args(args...));
        }

        void debug(std::string_view msg) { sink_it(log_level::debug, msg); }

        template<typename... Args>
        requires (sizeof...(Args) > 0)
        void error(std::format_string<Args...> fmt, Args&&... args) {
            sink_formatted(log_level::error, fmt.get(), std::make_format_args(args...));
        }

        void error(std::string_view msg) { sink_it(log_level::error, msg); }

    private:
        void sink_formatted(log_level level, std::string_view fmt, const std::format_args& args) {
            try {
                std::string formatted = std::vformat(std::string_view(fmt), args);

                sink_it(level, formatted);
            } catch (const std::exception& e) {
                // 安全的 fallback
                sink_it(log_level::error, std::string("[Logger Error] Format failed: ") + e.what());
            }
        }
    };
}