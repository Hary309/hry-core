#pragma once

#include <memory>
#include <string>

#include "Hry/Logger/Logger.hpp"
#include "Hry/Namespace.hpp"

#include "Events/EventManager.hpp"

HRY_NS_BEGIN

class LoggerFactory
{
private:
    inline static std::string _logFilePath;
    inline static EventManager* _eventMgr;

public:
    static void Init(std::string logFilePath, EventManager& eventMgr);
    static void WriteLine(Logger::Level level, std::string_view module, std::string_view msg);

    static std::unique_ptr<Logger> GetLogger(const char* moduleName);
};

HRY_NS_END
