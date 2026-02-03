#include <filesystem>
#include <iostream>
#include <cstdlib> // for std::getenv

#ifdef _WIN32
#include <windows.h>
#endif

#include "modian/core/engine/pinyin_engine.h"
#include "modian/core/logger/logger_service.h"
#include "modian/logger/spdlog_logger.h"
#include "server.h"

std::string get_dict_path() {
    std::string home;
#ifdef _WIN32
    if (auto p = std::getenv("USERPROFILE")) home = p;
#else
    if (auto p = std::getenv("HOME")) home = p;
#endif
    if (home.empty()) {
        home = ".";
    }

    auto path = std::filesystem::path(home) / "Modian" / "Local" / "pinyin_dictionary.txt";
    return path.string();
}

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    modian::inkstone::core::logger_service::update_logger([](){
        return std::make_shared<modian::inkstone::infra::logger::spdlog_logger>();
    });

    try {
        const modian::inkstone::server inkstone_server{
            modian::inkstone::core::lazy_load_dictionary<modian::inkstone::core::pinyin_engine>(get_dict_path())
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