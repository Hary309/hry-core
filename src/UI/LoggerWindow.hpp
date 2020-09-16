#pragma once

#include <vector>

#include <imgui.h>

#include "Hry/Namespace.hpp"

#include "Events/InternalEventHandler.hpp"

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

    ImGuiTextFilter _filter;
    std::vector<int> _filteredLines;

    std::vector<Line> _lines;

    float _opacity = 1.f;

public:
    LoggerWindow(InternalEventHandler& eventMgr);

    void imguiRender();

    void setEnabled(bool enabled) { _enabled = enabled; }
    void setOpacity(float opacity) { _opacity = opacity; }

private:
    static void renderLine(const Line& line);
    void onLog(const std::string& msg, Logger::Level level);
};

HRY_NS_END