#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

#include "scriptorium/felt/infra/utils/string_utils.h"
#include "scriptorium/core/engine/pinyin_engine.h"
#include "logger_guard.h"
#include "scriptorium/felt/infra/logger/spdlog_logger.h"
#include "server.h"

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    scriptorium::inkstone::logger_guard<scriptorium::felt::infra::logger::spdlog_logger> log_guard{"inkstone"};

    try {
        scriptorium::inkstone::server inkstone_server{
            scriptorium::inkstone::core::lazy_load_dictionary<scriptorium::inkstone::core::pinyin_engine>(scriptorium::felt::infra::utils::get_dict_path())
        };
        inkstone_server.run();
    } catch (const std::exception& e) {
        if (scriptorium::felt::core::logger_service::logger()) {
            scriptorium::felt::core::logger_service::logger()->error("Fatal Error: {}", e.what());
        } else {
            std::cerr << "Fatal Error (Logger failed): " << e.what() << std::endl;
        }
        return 1;
    }

    return 0;
}
