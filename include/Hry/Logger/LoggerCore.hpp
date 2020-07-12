#pragma once

#include <string>
#include <memory>

namespace hry::logger
{

class ModuleLogger;

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
    void writeLine(Level level, const char* msg, const char* module);

    [[nodiscard]] std::unique_ptr<ModuleLogger> createModuleLogger(const char* moduleName);

private:
    void formatTime();
};

}
