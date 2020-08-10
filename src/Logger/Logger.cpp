#include "Hry/Logger/Logger.hpp"

#include "Hry/Namespace.hpp"

#include "Logger/LoggerFactory.hpp"

HRY_NS_BEGIN

void Logger::log(Level level, std::string_view msg)
{
    LoggerFactory::WriteLine(level, _moduleName, msg);
}

HRY_NS_END
