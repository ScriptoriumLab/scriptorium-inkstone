add_library(modian_inkstone_core STATIC
    src/dictionary/dictionary.cpp
    src/engine/pinyin_engine.cpp
)

target_link_libraries(modian_inkstone_core PRIVATE
    modian::common::core::logger
)

target_include_directories(modian_inkstone_core PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

add_library(modian::inkstone::core ALIAS modian_inkstone_core)
