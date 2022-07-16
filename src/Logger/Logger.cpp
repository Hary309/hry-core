/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#include "Hry/Logger/Logger.hpp"

#include "Logger/LoggerFactory.hpp"

namespace hry
{
void Logger::log_msg(Level level, std::string_view msg)
{
    LoggerFactory::WriteLine(level, _moduleName, msg);
}
}
