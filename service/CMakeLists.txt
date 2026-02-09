include(FetchContent)

FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)
FetchContent_MakeAvailable(json)

add_library(modian_inkstone_service STATIC
    src/input_protocol_service.cpp
)

target_link_libraries(modian_inkstone_service PRIVATE
    modian_inkstone_core
    nlohmann_json::nlohmann_json
)

target_include_directories(modian_inkstone_service PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
