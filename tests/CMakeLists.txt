include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.12.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(
    modian_inkstone_tests
    test_main.cpp
    core/engine/input_engine_test.cpp
)

target_include_directories(modian_inkstone_tests PRIVATE ${CMAKE_SOURCE_DIR}/tests/unit_tests/include)

target_link_libraries(
    modian_inkstone_tests
    modian_inkstone_core
    modian_inkstone_manager
    gtest_main
    gmock_main
)

target_compile_definitions(modian_inkstone_tests PRIVATE PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")
