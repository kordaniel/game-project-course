cmake_minimum_required(VERSION 3.16)

# Disable in-source builds
if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
    message(FATAL_ERROR "
FATAL ERROR: In-source builds are not allowed.
             Specify a separate build dir with the -B flag for build files.
")
endif()

# Include & config fetchcontent
include(FetchContent)
set(FETCHCONTENT_QUIET OFF)
set(FETCHCONTENT_UPDATES_DISCONNECTED ON)

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

# GLM
#add_compile_definitions(GLM_FORCE_MESSAGES) # Display information at build time
add_compile_definitions(GLM_FORCE_RADIANS)
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 0.9.9.7
    GIT_PROGRESS TRUE
)

# SDL2
FetchContent_Declare(
    sdl2-main
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG 505d6a4a052592b2676f87456c1f564daa8d2c50 # Version 2.0.22
    GIT_PROGRESS TRUE
)

# SDL_ttf
FetchContent_Declare(
    sdl2-ttf
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
    GIT_TAG 81f9e9b7436a649f7cb7440759234b9cbfdc0c50
    GIT_PROGRESS TRUE
)

# SDL2_image
FetchContent_Declare(
    sdl2-image
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
    GIT_TAG a623e629776e6ae7294d73a0f31128cf0031fe4f
    GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(fmt glm sdl2-main sdl2-ttf sdl2-image)

# Set sourcecode path, both cpp and header-files live here
add_subdirectory("${PROJECT_SOURCE_DIR}/src")

configure_file("${CMAKE_SOURCE_DIR}/configuration/Config.hpp.in" "${PROJECT_SOURCE_DIR}/src/Config.hpp" @ONLY)

# Build tests if we are building a DEBUG build
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    enable_testing()
    add_subdirectory("${PROJECT_SOURCE_DIR}/test")
endif()
