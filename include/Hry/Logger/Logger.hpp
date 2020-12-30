/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include <sstream>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

#include <fmt/core.h>
#include <fmt/format.h>

#include "Hry/Export.hpp"
#include "Hry/Namespace.hpp"

HRY_NS_BEGIN

/**
 * @brief Logger class
 * 
 * Useful for logging actions and printing debug information
 * 
 */
class Logger final
{
public:
    /**
     * @brief Log level
     */
    enum class Level
    {
        Info,
        Warning,
        Error
    };

private:
    std::string _moduleName;

public:
    /**
     * @brief Construct a new Logger object
     * 
     * @param moduleName Name to identify log message
     */
    explicit Logger(std::string moduleName) : _moduleName(std::move(moduleName)) {}

    /**
     * @brief Write log message
     * 
     * @param level Level of message
     * @param msg Message content
     */
    HRY_API void log(Level level, std::string_view msg);

    /**
     * @brief Write log message with formatting
     * 
     * @tparam Args Arguments for formatting
     * @param level Level of message
     * @param format Formatting string
     * @param args Arguments for formatting
     */
    template<typename... Args>
    void log(Level level, std::string_view format, Args&&... args)
    {
        log(level, fmt::format(format, std::forward<Args>(args)...));
    }

    /**
     * @brief Write log message with info level
     * 
     * @tparam Args Arguments for formatting
     * @param format Formatting string
     * @param args Arguments for formatting
     */
    template<typename... Args>
    void info(std::string_view format, Args&&... args)
    {
        log(Level::Info, format, std::forward<Args>(args)...);
    }

    /**
     * @brief Write log message with warning level
     * 
     * @tparam Args Arguments for formatting
     * @param format Formatting string
     * @param args Arguments for formatting
     */
    template<typename... Args>
    void warning(std::string_view format, Args&&... args)
    {
        log(Level::Warning, format, std::forward<Args>(args)...);
    }

    /**
     * @brief Write log message with error level
     * 
     * @tparam Args Arguments for formatting
     * @param format Formatting string
     * @param args Arguments for formatting
     */
    template<typename... Args>
    void error(std::string_view format, Args&&... args)
    {
        log(Level::Error, format, std::forward<Args>(args)...);
    }
};

HRY_NS_END
