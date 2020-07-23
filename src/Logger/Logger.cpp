#include "Hry/Logger/Logger.hpp"

#include "Logger/LoggerFactory.hpp"

namespace hry
{
void Logger::log(Level level, const char* msg)
{
    LoggerFactory::WriteLine(level, msg, _moduleName.c_str());
}

} // namespace hry
