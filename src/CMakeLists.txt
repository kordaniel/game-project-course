cmake_minimum_required(VERSION 3.16)

set(headers
    "Color.hpp"
    "Constants.hpp"
    "Font.hpp"
    "Helpers.hpp"
    "Image.hpp"
    "Input.hpp"
    "Logger.hpp"
    "ResourceManager.hpp"
)

set(sources
    "Color.cpp"
    "Constants.cpp"
    "Font.cpp"
    "Helpers.cpp"
    "Image.cpp"
    "Input.cpp"
    "Logger.cpp"
    "ResourceManager.cpp"
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
    PRIVATE "${sdl2-main_SOURCE_DIR}/include"
    PRIVATE "${sdl2-ttf_SOURCE_DIR}"
    PRIVATE "${sdl2-image_SOURCE_DIR}"
)

# Use header-only fmt:
target_link_libraries("${EXECNAME}" PRIVATE
    PRIVATE fmt::fmt-header-only
    PRIVATE SDL2::SDL2
    PRIVATE SDL2_ttf
    PRIVATE SDL2_image
)

add_custom_command(
    TARGET "${EXECNAME}" POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy "${CMAKE_SOURCE_DIR}/res/fonts/indie-flower/IndieFlower-Regular.ttf" $<TARGET_FILE_DIR:${EXECNAME}>/res/fonts/indieflower.ttf
    COMMAND "${CMAKE_COMMAND}" -E copy "${CMAKE_SOURCE_DIR}/res/fonts/patrick-hand/PatrickHand-Regular.ttf" $<TARGET_FILE_DIR:${EXECNAME}>/res/fonts/patrickhand.ttf
    COMMAND "${CMAKE_COMMAND}" -E copy "${CMAKE_SOURCE_DIR}/res/fonts/permanent-marker/PermanentMarker-Regular.ttf" $<TARGET_FILE_DIR:${EXECNAME}>/res/fonts/permanentmarker.ttf
    COMMAND "${CMAKE_COMMAND}" -E copy "${CMAKE_SOURCE_DIR}/res/fonts/rubik-bubbles/RubikBubbles-Regular.ttf" $<TARGET_FILE_DIR:${EXECNAME}>/res/fonts/rubikbubbles.ttf
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_SOURCE_DIR}/res/images" $<TARGET_FILE_DIR:${EXECNAME}>/res/images
    COMMENT "Copied all resources to the directory res/ in relation to the executable"
)
