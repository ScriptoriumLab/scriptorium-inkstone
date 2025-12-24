#include <windows.h>
#include <iostream>

#include "modian/core/logger/logger_service.h"
#include "modian/logger/spdlog_logger.h"
#include "server.h"

int main() {
    SetConsoleOutputCP(CP_UTF8);

    modian::inkstone::core::logger_service::update_logger([](){
        return std::make_shared<modian::inkstone::infra::logger::spdlog_logger>();
    });

    try {
        const modian::inkstone::server inkstone_server;
        inkstone_server.run();
    } catch (const std::exception& e) {
        modian::inkstone::core::logger_service::logger()->error("Fatal Error: {}", e.what());
        return 1;
    }

    return 0;
}