add_library(scriptorium_inkstone_test_utils INTERFACE
    include/scriptorium/tests/utils/test_input_protocol_pipe_client.h
    include/scriptorium/tests/utils/test_ui_protocol_pipe_client.h
)

target_include_directories(scriptorium_inkstone_test_utils INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(scriptorium::inkstone::test_utils ALIAS scriptorium_inkstone_test_utils)