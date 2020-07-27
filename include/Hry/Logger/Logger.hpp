#pragma once

#include <sstream>
#include <string>
#include <type_traits>
#include <utility>

#include "Hry/Export.hpp"
#include "Hry/Namespace.hpp"

HRY_NS_BEGIN

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
    explicit Logger(std::string moduleName) : _moduleName(std::move(moduleName)) {}

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
        log(Level::Info, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void warning(Args&&... args)
    {
        log(Level::Warning, std::forward<Args>(args)...);
    }
    template<typename... Args>
    void error(Args&&... args)
    {
        log(Level::Error, std::forward<Args>(args)...);
    }
};

HRY_NS_END
