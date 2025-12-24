#include "modian/logger/spdlog_logger.h"

#include <iostream>
#include <filesystem>

#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/spdlog.h>

namespace fs = std::filesystem;

namespace modian::inkstone::infra::logger {

    spdlog_logger::spdlog_logger() {
        const std::string logger_name = "inkstone_logger";

        if (const auto existing_logger = spdlog::get(logger_name)) {
            logger_ = existing_logger;
            spdlog::set_default_logger(logger_);
            spdlog::set_level(spdlog::level::debug);
            return;
        }

        try {
            char* userprofile_raw{nullptr};
            size_t size = 0;

            if (_dupenv_s(&userprofile_raw, &size, "USERPROFILE") != 0 || userprofile_raw == nullptr) {
                std::cerr << "[Fatal] Failed to retrieve USERPROFILE environment variable." << std::endl;
                return;
            }

            const fs::path home_dir = userprofile_raw;
            free(userprofile_raw);

            const fs::path log_dir = home_dir / "Modian" / "Log";

            const fs::path log_path = log_dir / "modian-inkstone.log";

            if (!fs::exists(log_dir)) {
                std::error_code ec;
                fs::create_directories(log_dir, ec);
                if (ec) {
                    std::cerr << "[Fatal] Failed to create log directory: " << ec.message() << std::endl;
                    return;
                }
            }

            logger_ = spdlog::rotating_logger_mt(logger_name, log_path.string(), 1024 * 1024 * 5, 3);

            spdlog::set_default_logger(logger_);
            spdlog::set_level(spdlog::level::debug);

            spdlog::flush_on(spdlog::level::info);

            logger_->info("=== Inkstone Server Started ===");
            logger_->info("Logger initialized at: {}", log_path.string());

        } catch (const spdlog::spdlog_ex& ex) {
            std::cerr << "[Fatal] Log initialization failed: " << ex.what() << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "[Fatal] Unexpected error in logger init: " << e.what() << std::endl;
        }
    }

    spdlog_logger::~spdlog_logger() {
        logger_->info("=== Inkstone Server Stopping ===");
        spdlog::shutdown();
    }

    void spdlog_logger::sink_it(core::log_level level, std::string_view msg) {
        if (!logger_) return;

        switch (level) {
        case core::log_level::debug:
            logger_->debug(msg);
            break;
        case core::log_level::info:
            logger_->info(msg);
            break;
        case core::log_level::error:
            logger_->error(msg);
            break;
        }

        if (level == core::log_level::error) logger_->flush();
    }
}