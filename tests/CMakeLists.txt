include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.12.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(
        modian_inkstone_test
        test_main.cpp
        test_template.cpp
)

target_link_libraries(
        modian_inkstone_test
        modian_inkstone_lib
        gtest_main
        gmock_main
)
