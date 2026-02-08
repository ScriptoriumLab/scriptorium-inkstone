add_executable(
    modian_inkstone_unit_tests
    test_main.cpp
    core/engine/input_engine_test.cpp
        core/protocol/instruction_test.cpp
    manager/candidate_manager_test.cpp
    infra/utils/utils_test.cpp
)

target_link_libraries(
    modian_inkstone_unit_tests
    modian_inkstone_core
    modian_inkstone_manager
    modian_inkstone_infra_utils
    gtest_main
    gmock_main
)

target_compile_definitions(modian_inkstone_unit_tests PRIVATE PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")
