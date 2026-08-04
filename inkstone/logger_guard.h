#pragma once

#include "scriptorium/felt/core/logger/logger_service.h"

namespace scriptorium::inkstone {
    template<typename logger>
    struct logger_guard {
        template<typename... Args>
        logger_guard(Args&&... args) {
            auto instance = std::make_shared<logger>(std::forward<Args>(args)...);
            felt::core::logger_service::update_logger([instance](){
                return instance;
            });
        }

        logger_guard(const logger_guard&) = delete;
        logger_guard(logger_guard&&) = delete;
        logger_guard operator=(const logger_guard&) = delete;
        logger_guard operator=(logger_guard&&) = delete;

        ~logger_guard() {
            felt::core::logger_service::shutdown();
        }
    };
}
