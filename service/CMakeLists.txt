add_library(modian_inkstone_service STATIC
    src/input_protocol_service.cpp
)

target_link_libraries(modian_inkstone_service PRIVATE
    modian_inkstone_core
)

target_include_directories(modian_inkstone_service PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
