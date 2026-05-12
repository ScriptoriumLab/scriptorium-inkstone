add_executable(modian_inkstone_unit_tests
    test_main.cpp
    core/engine/input_engine_test.cpp
    service/input_protocol_service_test.cpp
    service/ui_protocol_service_test.cpp
    manager/candidate_manager_test.cpp
)

target_link_libraries(modian_inkstone_unit_tests PRIVATE
    modian::common::core::observer
    modian::common::core::protocol
    modian::common::infra::utils
    modian::inkstone::core
    modian::inkstone::service
    modian::inkstone::manager
    gtest_main
    gmock_main
)

target_compile_definitions(modian_inkstone_unit_tests PRIVATE PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")
