#pragma once

#include <sstream>
#include <string>
#include <type_traits>
#include <utility>

#include <fmt/core.h>
#include <fmt/format.h>

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

    HRY_API void log(Level level, std::string_view msg);

    template<typename... Args>
    void log(Level level, std::string_view format, Args&&... args)
    {
        log(level, fmt::format(format, std::forward<Args>(args)...));
    }

    template<typename... Args>
    void info(std::string_view format, Args&&... args)
    {
        log(Level::Info, format, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void warning(std::string_view format, Args&&... args)
    {
        log(Level::Warning, format, std::forward<Args>(args)...);
    }
    template<typename... Args>
    void error(std::string_view format, Args&&... args)
    {
        log(Level::Error, format, std::forward<Args>(args)...);
    }
};

HRY_NS_END
