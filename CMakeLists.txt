cmake_minimum_required(VERSION 3.16)

# Disable in-source builds
if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
    message(FATAL_ERROR "
FATAL ERROR: In-source builds are not allowed.
             Specify a separate build dir with the -B flag for build files.
")
endif()

# Includes
include(FetchContent)

# Set build path to ./bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

# Set the name (for the binary) and version
project(gameproj VERSION 0.1)

# Set the C++ version to use standard C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set compileflags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wshadow -pedantic -Wpedantic -Wunused-result -Wnon-virtual-dtor -Wcast-align -Woverloaded-virtual -Wconversion -Wsign-conversion -fdelete-null-pointer-checks -Wnull-dereference -Wdouble-promotion")
# For C (SDL requires -fPIC)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")

# Set flags for apple clang
if(CMAKE_CXX_COMPILER_ID MATCHES "AppleClang")
    add_compile_options("-Wshadow-field-in-constructor" "-Wsign-compare")
endif()

# Append RELEASE / DEBUG specific flags
list(APPEND CXX_FLAGS_RELEASE "-Werror" "-flto") # -O3 -DNDEBUG is set by cmake
#list(APPEND CXX_FLAGS_DEBUG "-fsanitize=undefined" "-fsanitize=address" "-O0") # -g is set by cmake

# Set linking flags, atleast MacOS/Xcode clang (or only ld?) requires the fsanitize flags to be set
#set(CXX_LDFLAGS_DEBUG "-fsanitize=undefined" "-fsanitize=address" "-rdynamic")

# Fetch and build libraries:
# -------------------------

# Fmt
FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG master
    GIT_PROGRESS TRUE
)

# Setup GoogleTest testing framework in DEBUG builds
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/6b74da4757a549563d7c37c8fae3e704662a043b.zip
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif()

# SDL2
# Version 2.0.22
FetchContent_Declare(
    SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG 727eef7064e02aea89281493d0c5f16ad9e3c16f
    GIT_PROGRESS TRUE
)

# SDL_ttf
FetchContent_Declare(
    SDL2_TTF
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
    GIT_TAG 81f9e9b7436a649f7cb7440759234b9cbfdc0c50
    GIT_PROGRESS TRUE
)

# SDL2_image
FetchContent_Declare(
    SDL2_IMAGE
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
    GIT_TAG a623e629776e6ae7294d73a0f31128cf0031fe4f
    GIT_PROGRESS TRUE
)

# cmake/fetchcontent recommends to use lowercased names
FetchContent_MakeAvailable(fmt sdl2 sdl2_ttf sdl2_image)

# Set sourcecode path, both cpp and header-files live here
add_subdirectory("${PROJECT_SOURCE_DIR}/src")

configure_file("${CMAKE_SOURCE_DIR}/configuration/Config.hpp.in" "${PROJECT_SOURCE_DIR}/src/Config.hpp" @ONLY)

# Build tests if we are building a DEBUG build
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    enable_testing()
    add_subdirectory("${PROJECT_SOURCE_DIR}/test")
endif()
