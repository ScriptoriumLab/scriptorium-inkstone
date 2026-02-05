#include <filesystem>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

#include "modian/infra/utils/utils.h"
#include "modian/core/engine/pinyin_engine.h"
#include "modian/core/logger/logger_service.h"
#include "modian/logger/spdlog_logger.h"
#include "server.h"

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    modian::inkstone::core::logger_service::update_logger([](){
        return std::make_shared<modian::inkstone::infra::logger::spdlog_logger>();
    });

    try {
        modian::inkstone::server inkstone_server{
            modian::inkstone::core::lazy_load_dictionary<modian::inkstone::core::pinyin_engine>(modian::inkstone::infra::utils::get_dict_path())
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