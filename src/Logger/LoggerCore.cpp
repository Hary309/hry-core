#include "LoggerCore.hpp"

#include <fstream>
#include <filesystem>
#include <iomanip>
#include <ctime>

namespace hry::logger
{

static std::string logFilePath;


void LoggerCore::init(const char* logPath) 
{
    logFilePath = logPath;

    if (std::filesystem::exists(logFilePath))
    {
        std::filesystem::remove(logFilePath);
    }
}

void LoggerCore::writeLine(const char* msg, const char* levelName, const char* module) 
{
    std::ofstream logFile(logFilePath, std::ios::app);

    if (!logFile.is_open())
    {
        return;
    }

    char timeBuffer[16];
    time_t tm = std::time(nullptr);
    strftime(timeBuffer, 16, "", std::localtime(&tm));

    char buffer[128];
    snprintf(buffer, 120, "[%16s] [%4s] [%8s] ", timeBuffer, levelName, module);

    logFile << buffer;
}

}