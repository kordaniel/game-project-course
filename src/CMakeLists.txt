cmake_minimum_required(VERSION 3.16)

set(headers
    "Color.hpp"
    "Constants.hpp"
    "Font.hpp"
    "Game.hpp"
    "GameObject.hpp"
    "Geometry.hpp"
    "Helpers.hpp"
    "Image.hpp"
    "Input.hpp"
    "Label.hpp"
    "Logger.hpp"
    "Menu.hpp"
    "Mixer.hpp"
    "Music.hpp"
    "Physics.hpp"
    "Renderer.hpp"
    "ResourceManager.hpp"
    "Sdl2.hpp"
    "Sound.hpp"
    "Texture.hpp"
    "Timetools.hpp"
    "Transform.hpp"
    "Window.hpp"
)

set(sources
    "Color.cpp"
    "Constants.cpp"
    "Font.cpp"
    "Game.cpp"
    "GameObject.cpp"
    "Geometry.cpp"
    "Helpers.cpp"
    "Image.cpp"
    "Input.cpp"
    "Label.cpp"
    "Logger.cpp"
    "Main.cpp"
    "Menu.cpp"
    "Mixer.cpp"
    "Music.cpp"
    "Physics.cpp"
    "Renderer.cpp"
    "ResourceManager.cpp"
    "Sdl2.cpp"
    "Sound.cpp"
    "Texture.cpp"
    "Timetools.cpp"
    "Transform.cpp"
    "Window.cpp"
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
    PRIVATE "${sdl2-mixer_SOURCE_DIR}/include"
)

# Use header-only fmt and glm:
target_link_libraries("${EXECNAME}" PRIVATE
    PRIVATE fmt::fmt-header-only
    PRIVATE glm
    PRIVATE SDL2::SDL2
    PRIVATE SDL2_ttf
    PRIVATE SDL2_image
    PRIVATE SDL2_mixer
)

add_custom_command(
    TARGET "${EXECNAME}" POST_BUILD
# Fonts
    COMMAND "${CMAKE_COMMAND}" -E copy "${CMAKE_SOURCE_DIR}/res/fonts/indie-flower/IndieFlower-Regular.ttf" $<TARGET_FILE_DIR:${EXECNAME}>/res/fonts/indieflower.ttf
    COMMAND "${CMAKE_COMMAND}" -E copy "${CMAKE_SOURCE_DIR}/res/fonts/patrick-hand/PatrickHand-Regular.ttf" $<TARGET_FILE_DIR:${EXECNAME}>/res/fonts/patrickhand.ttf
    COMMAND "${CMAKE_COMMAND}" -E copy "${CMAKE_SOURCE_DIR}/res/fonts/permanent-marker/PermanentMarker-Regular.ttf" $<TARGET_FILE_DIR:${EXECNAME}>/res/fonts/permanentmarker.ttf
    COMMAND "${CMAKE_COMMAND}" -E copy "${CMAKE_SOURCE_DIR}/res/fonts/rubik-bubbles/RubikBubbles-Regular.ttf" $<TARGET_FILE_DIR:${EXECNAME}>/res/fonts/rubikbubbles.ttf
# Images
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_SOURCE_DIR}/res/images" $<TARGET_FILE_DIR:${EXECNAME}>/res/images
# Music
    COMMAND "${CMAKE_COMMAND}" -E copy "${CMAKE_SOURCE_DIR}/res/music/ogg-short-loopable-background-music/Beats_A.ogg" $<TARGET_FILE_DIR:${EXECNAME}>/res/musics/beats_a.ogg
    COMMAND "${CMAKE_COMMAND}" -E copy "${CMAKE_SOURCE_DIR}/res/music/ogg-short-loopable-background-music/Beats_D.ogg" $<TARGET_FILE_DIR:${EXECNAME}>/res/musics/beats_d.ogg
    COMMAND "${CMAKE_COMMAND}" -E copy "${CMAKE_SOURCE_DIR}/res/music/ogg-short-loopable-background-music/Effects_B.ogg" $<TARGET_FILE_DIR:${EXECNAME}>/res/musics/effects_b.ogg
    COMMAND "${CMAKE_COMMAND}" -E copy "${CMAKE_SOURCE_DIR}/res/music/ogg-short-loopable-background-music/Groovy_booty.ogg" $<TARGET_FILE_DIR:${EXECNAME}>/res/musics/groovy_booty.ogg
    COMMAND "${CMAKE_COMMAND}" -E copy "${CMAKE_SOURCE_DIR}/res/music/ogg-short-loopable-background-music/Light_Music.ogg" $<TARGET_FILE_DIR:${EXECNAME}>/res/musics/light_music.ogg
    COMMAND "${CMAKE_COMMAND}" -E copy "${CMAKE_SOURCE_DIR}/res/music/ogg-short-loopable-background-music/Spaceship.ogg" $<TARGET_FILE_DIR:${EXECNAME}>/res/musics/spaceship.ogg
# Sounds
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_SOURCE_DIR}/res/sounds" $<TARGET_FILE_DIR:${EXECNAME}>/res/sounds

    COMMENT "Copied all resources to the directory res/ in relation to the executable"
)
