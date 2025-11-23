#pragma once

#include <string>
#include <string_view>
#include <format>

namespace modian::inkstone::core {

    enum class log_level { debug, info, error };

    class base_logger {
    public:
        virtual ~base_logger() = default;
        virtual void sink_it(log_level level, std::string_view msg) = 0;
        [[nodiscard]] virtual std::string_view type() const { return "base_logger"; }

        template<typename... Args>
        void info(std::format_string<Args...> fmt, Args&&... args) {
            format_and_sink(log_level::info, fmt, std::forward<Args>(args)...);
        }

        void info(std::string_view msg)  { sink_it(log_level::info, msg); }

        template<typename... Args>
        void debug(std::format_string<Args...> fmt, Args&&... args) {
            format_and_sink(log_level::debug, fmt, std::forward<Args>(args)...);
        }

        void debug(std::string_view msg) { sink_it(log_level::debug, msg); }

        template<typename... Args>
        void error(std::format_string<Args...> fmt, Args&&... args) {
            format_and_sink(log_level::error, fmt, std::forward<Args>(args)...);
        }

        void error(std::string_view msg) { sink_it(log_level::error, msg); }

    private:
        template<typename... Args>
        void format_and_sink(log_level level, std::format_string<Args...> fmt, Args&&... args) {
            try {
                std::string formatted = std::format(fmt, std::forward<Args>(args)...);
                sink_it(level, formatted);
            } catch (const std::format_error& e) {
                sink_it(log_level::error, std::format("[Logger Error] Format failed: {}", e.what()));
            }
        }
    };
}