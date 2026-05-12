#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

#include "modian/common/infra/utils/string_utils.h"
#include "modian/core/engine/pinyin_engine.h"
#include "logger_guard.h"
#include "modian/common/infra/logger/spdlog_logger.h"
#include "server.h"

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    logger_guard<modian::common::infra::logger::spdlog_logger> log_guard{};

    try {
        modian::inkstone::server inkstone_server{
            modian::inkstone::core::lazy_load_dictionary<modian::inkstone::core::pinyin_engine>(modian::common::infra::utils::get_dict_path())
        };
        inkstone_server.run();
    } catch (const std::exception& e) {
        if (modian::inkstone::core::logger_service::logger()) {
            modian::inkstone::core::logger_service::logger()->error("Fatal Error: {}", e.what());
        } else {
            std::cerr << "Fatal Error (Logger failed): " << e.what() << std::endl;
        }
        return 1;
    }

    return 0;
}
