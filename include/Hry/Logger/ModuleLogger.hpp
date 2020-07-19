#pragma once

#include <string>
#include <sstream>

#include "LoggerCore.hpp"

namespace hry
{

class ModuleLogger
{
    friend std::unique_ptr<ModuleLogger> LoggerCore::createModuleLogger(const char *);

private:
    LoggerCore& _loggerCore;
    std::string _moduleName;

private:
    ModuleLogger(LoggerCore& loggerCore, const char* moduleName)
        : _loggerCore(loggerCore), _moduleName(std::string(moduleName))
    {
    }

public:
    template<typename... Args>
    void log(LoggerCore::Level level, Args&&... args)
    {
        static_assert(sizeof...(args) != 0, "You must pass some arguments!");

        std::stringstream ss;
        ((ss << args), ...);
        _loggerCore.writeLine(LoggerCore::Level::Info, ss.str().c_str(), _moduleName.c_str());
    }

    template<typename... Args>
    void info(Args&&... args)
    {
        log(LoggerCore::Level::Info, args...);
    }
    
    template<typename... Args>
    void warning(Args&&... args)
    {
        log(LoggerCore::Level::Warning,  args...);
    }
    template<typename... Args>
    void error(Args&&... args)
    {
        log(LoggerCore::Level::Error, args...);
    }
    
};

}
