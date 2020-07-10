#pragma once

namespace hry::logger
{

class LoggerCore
{
private:
    char _timeBuffer[128];

public:
    void init(const char* logFilePath);

    void writeLine(const char* msg, const char* levelName, const char* module);

private:
    void formatTime();
};

}
