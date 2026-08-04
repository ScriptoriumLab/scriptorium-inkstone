add_library(scriptorium_inkstone_core STATIC
    src/tokenizer/tokenizer.cpp
    src/dictionary/dictionary.cpp
    src/engine/pinyin_engine.cpp
)

target_link_libraries(scriptorium_inkstone_core PRIVATE
    scriptorium::felt::core::logger
)

target_include_directories(scriptorium_inkstone_core PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

add_library(scriptorium::inkstone::core ALIAS scriptorium_inkstone_core)
