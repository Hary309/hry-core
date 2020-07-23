#pragma once

#include <memory>
#include <string>

#include "Hry/Logger/Logger.hpp"
#include "Hry/Namespace.hpp"

HRY_NS_BEGIN

class LoggerFactory
{
private:
    inline static std::string _logFilePath;

public:
    static void Init(const char* logFilePath);
    static void WriteLine(Logger::Level level, const char* msg, const char* module);

    static std::unique_ptr<Logger> GetLogger(const char* moduleName);
};

HRY_NS_END
