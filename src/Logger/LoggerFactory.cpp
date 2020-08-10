#include "LoggerFactory.hpp"

#include <filesystem>
#include <fstream>

#include <fmt/chrono.h>
#include <fmt/core.h>
#include <fmt/format.h>

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

    std::string msg = fmt::format("Started logging to {}", _logFilePath);

    WriteLine(Logger::Level::Info, "core", msg);
}

void LoggerFactory::WriteLine(Logger::Level level, std::string_view module, std::string_view msg)
{
    std::ofstream logFile(_logFilePath, std::ios::app);

    if (!logFile.is_open())
    {
        return;
    }

    std::string levelName = [&level]() {
        switch (level)
        {
            case Logger::Level::Info: return "info";
            case Logger::Level::Warning: return "warn";
            case Logger::Level::Error: return "erro";
        }
    }();

    std::time_t t = std::time(nullptr);

    auto buffer =
        fmt::format("[{:%H:%M:%S}] [{}] [{}] {}", fmt::localtime(t), levelName, module, msg);

    logFile << buffer << std::endl;

#ifdef DEBUG
    puts(buffer.c_str());
#endif
}

std::unique_ptr<Logger> LoggerFactory::GetLogger(const char* moduleName)
{
    return std::make_unique<Logger>(moduleName);
}

HRY_NS_END