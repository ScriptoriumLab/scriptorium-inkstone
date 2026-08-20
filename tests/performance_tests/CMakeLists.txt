set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
set(BENCHMARK_INSTALL OFF CACHE BOOL "" FORCE)
set(BENCHMARK_DOWNLOAD_DEPENDENCIES OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    googlebenchmark
    URL https://github.com/google/benchmark/archive/refs/tags/v1.9.5.zip
)

FetchContent_MakeAvailable(googlebenchmark)

add_executable(
    scriptorium_inkstone_performance_tests
    server_benchmark.cpp
)

target_link_libraries(
    scriptorium_inkstone_performance_tests PRIVATE
    benchmark::benchmark
    scriptorium::felt::core::logger
    scriptorium::felt::infra::logger
    scriptorium::inkstone::server
    scriptorium::inkstone::core
    scriptorium::inkstone::test_utils
)

target_compile_definitions(scriptorium_inkstone_performance_tests PRIVATE
    PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}"
)

add_test(
    NAME scriptorium_inkstone_performance_tests
    COMMAND scriptorium_inkstone_performance_tests
)

set_tests_properties(
    scriptorium_inkstone_performance_tests
    PROPERTIES
        LABELS "inkstone-performance"
)
