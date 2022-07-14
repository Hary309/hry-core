/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#include "LoggerFactory.hpp"

#include "Hry/Logger/Logger.hpp"

#include <fmt/chrono.h>
#include <fmt/core.h>
#include <fmt/format.h>

#include <filesystem>
#include <fstream>
#include <utility>

namespace hry
{
void LoggerFactory::Init(const std::string& logFilePath, EventManager& eventMgr)
{
    _eventMgr = &eventMgr;
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

    auto buffer = fmt::format("[{:%H:%M:%S}] [{}] [{}] {}", fmt::localtime(t), levelName, module, msg);

    logFile << buffer << std::endl;

    _eventMgr->system.logSignal.call(buffer, level);
}

std::unique_ptr<Logger> LoggerFactory::GetLogger(const char* moduleName)
{
    return std::make_unique<Logger>(moduleName);
}
}