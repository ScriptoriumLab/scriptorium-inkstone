include(FetchContent)
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.17.0
)
FetchContent_MakeAvailable(spdlog)
add_compile_definitions(SPDLOG_HEADER_ONLY)

add_library(modian_inkstone_logger STATIC
    src/spdlog_logger.cpp
)

target_include_directories(modian_inkstone_logger PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(modian_inkstone_logger
    PRIVATE modian::inkstone::core
    PUBLIC spdlog::spdlog
)

add_library(modian::inkstone::logger ALIAS modian_inkstone_logger)
