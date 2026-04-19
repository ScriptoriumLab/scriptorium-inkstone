#pragma once

#include "modian/core/logger/logger_service.h"

template<typename logger>
struct logger_guard {
    logger_guard() {
        modian::inkstone::core::logger_service::update_logger([](){
            return std::make_shared<logger>();
        });
    }

    logger_guard(const logger_guard&) = delete;
    logger_guard(logger_guard&&) = delete;
    logger_guard operator=(const logger_guard&) = delete;
    logger_guard operator=(logger_guard&&) = delete;

    ~logger_guard() {
        modian::inkstone::core::logger_service::shutdown();
    }
};
