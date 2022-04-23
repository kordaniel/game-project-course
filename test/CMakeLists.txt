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

set(ColorTest "ColorTest")
set(ColorTestSources
    "ColorTest.cpp"
    "${CMAKE_SOURCE_DIR}/src/Color.cpp"
)

add_executable("${ColorTest}" "${ColorTestSources}")
add_test(
    NAME    "${ColorTest}"
    COMMAND "${ColorTest}"
)

set(GeometryTest "GeometryTest")
set(GeometryTestSources
    "GeometryTest.cpp"
    "${CMAKE_SOURCE_DIR}/src/Geometry.cpp"
)

add_executable("${GeometryTest}" "${GeometryTestSources}")
add_test(
    NAME    "${GeometryTest}"
    COMMAND "${GeometryTest}"
)

set(TimetoolsTest "TimetoolsTest")
set(TimetoolsTestSources
    "TimetoolsTest.cpp"
    "${CMAKE_SOURCE_DIR}/src/Timetools.cpp"
)

add_executable("${TimetoolsTest}" "${TimetoolsTestSources}")
add_test(
    NAME    "${TimetoolsTest}"
    COMMAND "${TimetoolsTest}"
)
