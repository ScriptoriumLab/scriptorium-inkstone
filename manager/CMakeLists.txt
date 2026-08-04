add_library(scriptorium_inkstone_manager STATIC
    src/engine_manager.cpp
    src/candidate_manager.cpp
    src/session_orchestrator.cpp
)

target_link_libraries(
    scriptorium_inkstone_manager
    PRIVATE
    scriptorium::felt::core::observer
    scriptorium::felt::core::protocol
    scriptorium::felt::core::logger
    scriptorium::inkstone::core
)

target_include_directories(scriptorium_inkstone_manager PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

add_library(scriptorium::inkstone::manager ALIAS scriptorium_inkstone_manager)
