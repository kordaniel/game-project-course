#include "Logger.hpp"
#include <iostream>

int main(void)
{
    Logger::Trace("Hello World!");
    Logger::Debug("Hello World!");
    Logger::Info("Hello World!");
    Logger::Critical("Hello World!");

    std::cout << "\nINFO:" << std::endl;
    Logger::SetLogLevel(Logger::Level::INFO);
    Logger::Trace("Hello World!");
    Logger::Debug("Hello World!");
    Logger::Info("Hello World!");
    Logger::Critical("Hello World!");

    std::cout << "\nTRACE:" << std::endl;
    Logger::SetLogLevel(Logger::Level::TRACE);
    Logger::Trace("Hello World!");
    Logger::Debug("Hello World!");
    Logger::Info("Hello World!");
    Logger::Critical("Hello World!");

    std::cout << "\nTest print: " << Logger::GetLogLevelAsString() << std::endl;
    return EXIT_SUCCESS;
}
