/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include "Hry/Export.hpp"

#include <fmt/core.h>
#include <fmt/format.h>

#include <sstream>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

namespace hry
{
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
    explicit Logger(std::string moduleName)
        : _moduleName(std::move(moduleName))
    {
    }

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
     * @param args Arguments
     */
    template<typename... Args>
    void log_format(Level level, Args&&... args)
    {
        log(level, fmt::format(std::forward<Args>(args)...));
    }

    /**
     * @brief Write log message with info level
     *
     * @tparam Args Arguments for formatting
     * @param args Arguments for formatting
     */
    template<typename... Args>
    void info(Args&&... args)
    {
        log_format(Level::Info, std::forward<Args>(args)...);
    }

    /**
     * @brief Write log message with warning level
     *
     * @tparam Args Arguments for formatting
     * @param args Arguments for formatting
     */
    template<typename... Args>
    void warning(Args&&... args)
    {
        log_format(Level::Warning, std::forward<Args>(args)...);
    }

    /**
     * @brief Write log message with error level
     *
     * @tparam Args Arguments for formatting
     * @param args Arguments for formatting
     */
    template<typename... Args>
    void error(Args&&... args)
    {
        log_format(Level::Error, std::forward<Args>(args)...);
    }
};
}
