add_library(modian_inkstone_ipc_server STATIC
    src/sync_named_pipe_server.cpp
    src/async_named_pipe_server.cpp
)

target_include_directories(modian_inkstone_ipc_server PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(
    modian_inkstone_ipc_server PRIVATE
    modian::inkstone::core
    modian::inkstone::infra_utils
)

add_library(modian::inkstone::ipc_server ALIAS modian_inkstone_ipc_server)
