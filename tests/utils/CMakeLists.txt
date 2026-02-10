add_library(modian_inkstone_test_utils INTERFACE
    include/modian/tests/utils/test_input_protocol_pipe_client.h
    include/modian/tests/utils/test_ui_protocol_pipe_client.h
)

target_include_directories(modian_inkstone_test_utils INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(modian::inkstone::test_utils ALIAS modian_inkstone_test_utils)