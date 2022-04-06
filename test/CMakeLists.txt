cmake_minimum_required(VERSION 3.16)

add_compile_options("${CXX_FLAGS}" "$<$<CONFIG:Debug>:${CXX_FLAGS_DEBUG}>")
add_link_options("$<$<CONFIG:Debug>:${CXX_LDFLAGS_DEBUG}>")
link_libraries(fmt::fmt-header-only)

include_directories(
    PRIVATE "${CMAKE_CURRENT_BINARY_DIR}"
    PRIVATE "${CMAKE_SOURCE_DIR}/src"
)

link_libraries(gtest gmock gtest_main)

set(LoggerTest "LoggerTest")
set(LoggerTestSources
    "LoggerTest.cpp"
    "${CMAKE_SOURCE_DIR}/src/Logger.cpp"
)

add_executable("${LoggerTest}" "${LoggerTestSources}")
add_test(
    NAME    "${LoggerTest}"
    COMMAND "${LoggerTest}"
)
