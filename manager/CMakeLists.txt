add_library(modian_inkstone_manager STATIC
    src/engine_manager.cpp
    src/candidate_manager.cpp
    src/session_orchestrator.cpp
)

target_link_libraries(
    modian_inkstone_manager
    PRIVATE
    modian::common::core::observer
    modian::common::core::protocol
    modian::inkstone::core
)

target_include_directories(modian_inkstone_manager PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

add_library(modian::inkstone::manager ALIAS modian_inkstone_manager)
