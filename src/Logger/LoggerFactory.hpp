#pragma once

#include <string>
#include <memory>

#include "Hry/Logger/Logger.hpp"

namespace hry
{

class LoggerFactory
{
private:
    inline static std::string _logFilePath;

public:
    static void Init(const char* logFilePath);
    static void WriteLine(Logger::Level level, const char* msg, const char* module);

    static std::unique_ptr<Logger> GetLogger(const char* moduleName);
};

}
