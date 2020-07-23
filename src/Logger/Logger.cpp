#include "Hry/Logger/Logger.hpp"

#include "Hry/Namespace.hpp"

#include "Logger/LoggerFactory.hpp"

HRY_NS_BEGIN

void Logger::log(Level level, const char* msg)
{
    LoggerFactory::WriteLine(level, msg, _moduleName.c_str());
}

HRY_NS_END
