add_executable(
    modian_inkstone
    server.cpp
    app.cpp
)

target_link_libraries(modian_inkstone PRIVATE
    modian_inkstone_manager
    modian_inkstone_ipc_server
    modian_inkstone_logger
    modian_inkstone_core
)

set_target_properties(modian_inkstone PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/ModianIME"
    OUTPUT_NAME "modian-inkstone"
)

set(RAW_USER_HOME "$ENV{USERPROFILE}")
file(TO_CMAKE_PATH "${RAW_USER_HOME}" USER_HOME)
set(MODIAN_ROOT_DIR "${USER_HOME}/Modian")
set(DICTIONARY_DIR "${USER_HOME}/Modian/Local")
set(MODIAN_LOG_DIR "${USER_HOME}/Modian/Log")
add_custom_target(set_up_modian_inkstone_env
    # 1. 创建目录 (如果不存在)
    COMMAND ${CMAKE_COMMAND} -E make_directory "${DICTIONARY_DIR}"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${MODIAN_LOG_DIR}"

    # 2. 拷贝词库文件 (copy_directory 会递归拷贝，copy_if_different 适合单文件)
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${modian-inkstone_SOURCE_DIR}/data" "${DICTIONARY_DIR}"

    # 3. (可选) 打印一条日志方便调试
    COMMENT "[Inkstone] Creating Modian root dir and copying dictionary assets to ${DICTIONARY_DIR}..."
    VERBATIM
)

add_custom_target(clean_up_modian_inkstone_env
    COMMAND ${CMAKE_COMMAND} -E remove_directory "${MODIAN_ROOT_DIR}"
    COMMENT "[Inkstone] Clean up Modian Env..."
    VERBATIM
)