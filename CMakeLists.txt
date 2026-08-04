cmake_minimum_required(VERSION 3.25)
project(scriptorium-inkstone)

set(CMAKE_CXX_STANDARD 23)

include(FetchContent)
FetchContent_Declare(
    scriptorium_felt
    GIT_REPOSITORY https://github.com/ScriptoriumLab/scriptorium-felt
    BRANCH master
)
FetchContent_MakeAvailable(scriptorium_felt)

include(CTest)

add_subdirectory(core)
add_subdirectory(manager)
add_subdirectory(inkstone)

if (BUILD_TESTING)
    add_subdirectory(tests)
endif ()
