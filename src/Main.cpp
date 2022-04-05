#include "Logger.hpp"


int main(void)
{
    Logger::Trace("Hello World!");
    Logger::Debug("Hello World!");
    Logger::Info("Hello World!");
    Logger::Critical("Hello World!");

    return EXIT_SUCCESS;
}
