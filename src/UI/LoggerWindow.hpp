#pragma once

#include <vector>

#include <imgui.h>

#include "Hry/Namespace.hpp"

#include "Events/EventManager.hpp"

HRY_NS_BEGIN

class LoggerWindow
{
    struct Line
    {
        std::string msg;
        Logger::Level level;
    };

private:
    bool _enabled = false;
    Sink<void(std::string msg, Logger::Level)> _onLog;

    ImGuiTextFilter _filter;
    std::vector<int> _filteredLines;

    std::vector<Line> _lines;

public:
    LoggerWindow(EventManager& eventMgr);

    void imguiRender();

    void setEnabled(bool enabled) { _enabled = enabled; }

private:
    static void renderLine(const Line& line);
    void onLog(std::string msg, Logger::Level level);
};

HRY_NS_END