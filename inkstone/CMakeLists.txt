add_library(scriptorium_inkstone_server server.cpp)

target_link_libraries(scriptorium_inkstone_server PUBLIC
    scriptorium::felt::core::observer
    scriptorium::felt::core::logger
    scriptorium::felt::service::protocol
    scriptorium::felt::infra::utils
    scriptorium::felt::infra::logger
    scriptorium::felt::infra::ipc
    scriptorium::inkstone::manager
    scriptorium::inkstone::core
)

target_include_directories(scriptorium_inkstone_server PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

add_library(scriptorium::inkstone::server ALIAS scriptorium_inkstone_server)

add_executable(
    scriptorium_inkstone
    app.cpp
)

target_link_libraries(scriptorium_inkstone PRIVATE
    scriptorium::inkstone::server
)

set_target_properties(scriptorium_inkstone PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/ScriptoriumLabIME"
    OUTPUT_NAME "scriptorium-inkstone"
)

add_executable(scriptorium::inkstone ALIAS scriptorium_inkstone)

set(RAW_USER_HOME "$ENV{USERPROFILE}")
file(TO_CMAKE_PATH "${RAW_USER_HOME}" USER_HOME)
set(SCRIPTORIUM_ROOT_DIR "${USER_HOME}/Scriptorium")
set(DICTIONARY_DIR "${USER_HOME}/Scriptorium/Local")
set(SCRIPTORIUM_LOG_DIR "${USER_HOME}/Scriptorium/Log")
add_custom_target(set_up_scriptorium_inkstone_env
    # 1. 创建目录 (如果不存在)
    COMMAND ${CMAKE_COMMAND} -E make_directory "${DICTIONARY_DIR}"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${SCRIPTORIUM_LOG_DIR}"

    # 2. 拷贝词库文件 (copy_directory 会递归拷贝，copy_if_different 适合单文件)
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${scriptorium-inkstone_SOURCE_DIR}/data" "${DICTIONARY_DIR}"

    # 3. (可选) 打印一条日志方便调试
    COMMENT "[Inkstone] Creating Scriptorium root dir and copying dictionary assets to ${DICTIONARY_DIR}..."
    VERBATIM
)

add_custom_target(clean_up_scriptorium_inkstone_env
    COMMAND ${CMAKE_COMMAND} -E remove_directory "${SCRIPTORIUM_ROOT_DIR}"
    COMMENT "[Inkstone] Clean up Scriptorium Env..."
    VERBATIM
)
