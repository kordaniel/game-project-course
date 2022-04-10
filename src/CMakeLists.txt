cmake_minimum_required(VERSION 3.16)

set(headers
    "Logger.hpp"
)

set(sources
    "Logger.cpp"
    "Main.cpp"
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set("EXECNAME" "${PROJECT_NAME}-dbg")
else()
    set("EXECNAME" "${PROJECT_NAME}")
endif()

add_executable("${EXECNAME}" "${sources}" "${headers}")

target_compile_options("${EXECNAME}" PRIVATE "${CXX_FLAGS}"
    "$<$<CONFIG:Debug>:${CXX_FLAGS_DEBUG}>"
    "$<$<CONFIG:Release>:${CXX_FLAGS_RELEASE}>"
)

target_link_options("${EXECNAME}" PRIVATE "$<$<CONFIG:Debug>:${CXX_LDFLAGS_DEBUG}>")

target_include_directories("${EXECNAME}"
    PRIVATE "${sdl2_SOURCE_DIR}/include"
    PRIVATE "${sdl2_ttf_SOURCE_DIR}"
    PRIVATE "${sdl2_image_SOURCE_DIR}"
)

# Use header-only fmt:
target_link_libraries("${EXECNAME}" PRIVATE
    fmt::fmt-header-only
    SDL2main
    SDL2-static
    SDL_ttf
    SDL2_image
)
