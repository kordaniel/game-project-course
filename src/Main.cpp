#include "Config.hpp" // defined in configuration/Config.hpp.in

#include "Logger.hpp"

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

void initialize([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    Logger::Info("Starting up. Version is: {}.{}.", gameproj_VERSION_MAJOR, gameproj_VERSION_MINOR);
    Logger::Info("Built with:\n"
        "\t\t\tCompiler name:    {}\n"
        "\t\t\tCompiler ID:      {}\n"
        "\t\t\tCompiler Version: {}",
        COMPILER_NAME,
        COMPILER_ID,
        COMPILER_VERSION
    );
#ifndef NDEBUG
    Logger::Info("This is a debug build without any optimizations");
#endif
    Logger::Info("Loglevel is set to: {}", Logger::GetLogLevelAsString());
}


int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    initialize(argc, argv);

    return EXIT_SUCCESS;
}
