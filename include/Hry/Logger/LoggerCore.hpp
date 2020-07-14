#pragma once

#include <string>
#include <memory>

#include "Hry/Export.hpp"

namespace hry::logger
{

class ModuleLogger;

// TODO: Hide init and createModuleLogger from visibility to api, use virtual class
class LoggerCore
{
public:
    enum class Level
    {
        Info,
        Warning,
        Error
    };

private:
    char _timeBuffer[128];
    std::string _logFilePath;

public:
    void init(const char* logFilePath);
    HRY_API void writeLine(Level level, const char* msg, const char* module);

    [[nodiscard]] std::unique_ptr<ModuleLogger> createModuleLogger(const char* moduleName);

private:
    void formatTime();
};

}
