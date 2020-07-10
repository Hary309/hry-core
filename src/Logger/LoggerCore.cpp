#include "Hry/Logger/LoggerCore.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <iomanip>
#include <ctime>
#include <memory>

#include "Hry/Logger/ModuleLogger.hpp"

namespace hry::logger
{

void LoggerCore::init(const char* logFilePath) 
{
    _logFilePath = logFilePath;

    if (std::filesystem::exists(_logFilePath))
    {
        std::filesystem::remove(_logFilePath);
    }

    writeLine(Level::Info, (std::string("Started logging to ") + _logFilePath).c_str(), "core");
}

void LoggerCore::writeLine(Level level, const char* msg, const char* module) 
{
    std::ofstream logFile(_logFilePath, std::ios::app);

    if (!logFile.is_open())
    {
        return;
    }

    char timeBuffer[16];
    time_t tm = std::time(nullptr);
    strftime(timeBuffer, 16, "%H:%M:%S", std::localtime(&tm));

    std::string levelName;
    levelName.reserve(5);

    switch (level)
    {
        case Level::Info: levelName = "info"; break;
        case Level::Warning: levelName = "warn"; break;
        case Level::Error: levelName = "erro"; break;
    }

    char buffer[128];
    snprintf(buffer, 128, "[%s] [%s] [%s] ", timeBuffer, levelName.c_str(), module);

    logFile << buffer;
    logFile << msg;
    logFile << std::endl;

#ifdef DEBUG
    std::cout << buffer << msg << '\n';
#endif
}

std::unique_ptr<ModuleLogger> LoggerCore::createModuleLogger(const char* moduleName) 
{
    return std::unique_ptr<ModuleLogger>(new ModuleLogger(*this, moduleName));
}

}