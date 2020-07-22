#pragma once

#include <string>
#include <sstream>

#include "Hry/Export.hpp"

namespace hry
{

class Logger
{
public:
    enum class Level
    {
        Info,
        Warning,
        Error
    };

private:
    std::string _moduleName;

public:
    Logger(const char* moduleName)
        : _moduleName(std::string(moduleName))
    {
    }

    HRY_API void log(Level level, const char* msg);

    template<typename... Args>
    void log(Level level, Args&&... args)
    {
        static_assert(sizeof...(args) != 0, "You must pass some arguments!");

        std::stringstream ss;
        ((ss << args), ...);
        log(level, ss.str().c_str());
    }

    template<typename... Args>
    void info(Args&&... args)
    {
        log(Level::Info, args...);
    }
    
    template<typename... Args>
    void warning(Args&&... args)
    {
        log(Level::Warning, args...);
    }
    template<typename... Args>
    void error(Args&&... args)
    {
        log(Level::Error, args...);
    }
};

}
