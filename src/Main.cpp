#include "Config.hpp" // defined in configuration/Config.hpp.in
#include "Logger.hpp"
#include "ResourceManager.hpp"
#include "Sdl2.hpp"
#include "Game.hpp"

#include <string>


/* ************************************************************************** *
 * BUGS: When toggling fullscreen with the key 'f' a SDL_WINDOWEVENT_SIZE_CHANGED
 *       event always occurs but a SDL_WINDOWEVENT_RESIZED event occurs seemingly
 *       only randomly. The latter is needed for the renderer to properly
 *       stretch the current surface/textures.
 * ************************************************************************** */

/* ************************************************************************** *
 * TODO: - Move position rectangle into class Texture. From: class Label
 *       - Implement a global switch to alter texture blending (for debugging)
 *       - Implement status for keys (pressed/repeat).
 *       - Use Mixer.hpp to handle Sound.hpp object, play & mix on assigned channels.
 *       - Abstract Sound objects ptrs (MIX_Chunk*) to own class and use from Mixer.
 * ************************************************************************** */

bool
initialize([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    Logger::Info("Starting up. Version is: {}.{}.", gameproj_VERSION_MAJOR, gameproj_VERSION_MINOR);
#ifndef NDEBUG
    Logger::Debug("Built with:\n"
        "\t\t\tCompiler name:    {}\n"
        "\t\t\tCompiler ID:      {}\n"
        "\t\t\tCompiler Version: {}",
        COMPILER_NAME,
        COMPILER_ID,
        COMPILER_VERSION
    );
    Sdl2::LogVersion();
    Sdl2::LogTtfVersion();
    Sdl2::LogImageVersion();
    Sdl2::LogMixerVersion();
    Logger::Info("This is a debug build without any optimizations");
#endif
    Logger::Info("Loglevel is set to: {}", Logger::GetLogLevelAsString());

    if (!Sdl2::Initialize()) {
        return false;
    }

    return true;
}

int
main(int argc, char* argv[])
{
    const std::string SCREEN_TITLE("Game project");
    constexpr int     SCREEN_WIDTH  = 1280;
    constexpr int     SCREEN_HEIGHT = 720;

    if (!initialize(argc, argv)) {
        Logger::Critical("Errors while initializing, terminating!");
        return EXIT_FAILURE;
    }

    ResourceManager resourceManager;
    Sdl2 sdl2Subsystem(SCREEN_TITLE, SCREEN_WIDTH, SCREEN_HEIGHT);
    Game game(sdl2Subsystem, resourceManager);

    game.Run();

    return EXIT_SUCCESS;
}
