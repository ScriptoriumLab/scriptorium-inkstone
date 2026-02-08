add_library(modian_inkstone_core STATIC
    src/engine/pinyin_engine.cpp
    src/logger/console_logger.cpp
    src/logger/logger_service.cpp
        src/protocol/instruction.cpp
)

target_include_directories(modian_inkstone_core PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)