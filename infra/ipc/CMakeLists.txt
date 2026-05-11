add_library(modian_inkstone_ipc_server STATIC)

if (WIN32)
    target_sources(modian_inkstone_ipc_server PRIVATE
        src/win/sync_named_pipe_server.cpp
        src/win/async_named_pipe_server.cpp
        src/win/ipc_server_factory.cpp
    )
endif()

target_include_directories(modian_inkstone_ipc_server PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(
    modian_inkstone_ipc_server PRIVATE
    modian::common::core::ipc
    modian::common::infra::utils
    modian::inkstone::core
)

add_library(modian::inkstone::ipc_server ALIAS modian_inkstone_ipc_server)
