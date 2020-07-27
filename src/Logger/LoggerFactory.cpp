#include "LoggerFactory.hpp"

#include <array>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>

#include "Hry/Logger/Logger.hpp"
#include "Hry/Namespace.hpp"

HRY_NS_BEGIN

void LoggerFactory::Init(const char* logFilePath)
{
    _logFilePath = logFilePath;

    if (std::filesystem::exists(_logFilePath))
    {
        std::filesystem::remove(_logFilePath);
    }

    WriteLine(
        Logger::Level::Info, (std::string("Started logging to ") + _logFilePath).c_str(), "core");
}

void LoggerFactory::WriteLine(Logger::Level level, const char* msg, const char* module)
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
        case Logger::Level::Info: levelName = "info"; break;
        case Logger::Level::Warning: levelName = "warn"; break;
        case Logger::Level::Error: levelName = "erro"; break;
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

std::unique_ptr<Logger> LoggerFactory::GetLogger(const char* moduleName)
{
    return std::make_unique<Logger>(moduleName);
}

HRY_NS_END