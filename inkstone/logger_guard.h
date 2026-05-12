#pragma once

#include "modian/common/core/logger/logger_service.h"

namespace modian::inkstone {
    template<typename logger>
    struct logger_guard {
        logger_guard() {
            common::core::logger_service::update_logger([](){
                return std::make_shared<logger>();
            });
        }

        logger_guard(const logger_guard&) = delete;
        logger_guard(logger_guard&&) = delete;
        logger_guard operator=(const logger_guard&) = delete;
        logger_guard operator=(logger_guard&&) = delete;

        ~logger_guard() {
            common::core::logger_service::shutdown();
        }
    };
}
