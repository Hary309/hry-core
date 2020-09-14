#include "LoggerWindow.hpp"

#include "Hry/Logger/Logger.hpp"

#include "imgui.h"

HRY_NS_BEGIN

LoggerWindow::LoggerWindow(InternalEventHandler& eventHandler)
{
    eventHandler.system.onLog.connect<&LoggerWindow::onLog>(this);
}

void LoggerWindow::imguiRender()
{
    if (!_enabled)
    {
        return;
    }

    if (!ImGui::Begin("Logs"))
    {
        ImGui::End();
        return;
    }

    // if value changed
    if (_filter.Draw("Filter", -100.0f))
    {
        _filteredLines.clear();

        for (size_t i = 0; i < _lines.size(); ++i)
        {
            const auto& msg = _lines[i].msg;

            // use &* to get raw pointer
            if (_filter.PassFilter(&*msg.begin(), &*msg.end()))
            {
                _filteredLines.push_back(i);
            }
        }
    }

    ImGui::Separator();

    ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

    if (_filter.IsActive())
    {
        ImGuiListClipper clipper;

        clipper.Begin(_filteredLines.size());
        while (clipper.Step())
        {
            for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
            {
                renderLine(_lines[_filteredLines[i]]);
            }
        }
        clipper.End();
    }
    else
    {
        ImGuiListClipper clipper;

        clipper.Begin(_lines.size());
        while (clipper.Step())
        {
            for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
            {
                renderLine(_lines[i]);
            }
        }
        clipper.End();
    }

    ImGui::PopStyleVar();

    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
    {
        ImGui::SetScrollHereY(1.0f);
    }

    ImGui::EndChild();
    ImGui::End();
}

void LoggerWindow::renderLine(const Line& line)
{
    const ImVec4 color = [&line]() {
        switch (line.level)
        {
            case Logger::Level::Warning: return ImColor(255, 255, 0);
            case Logger::Level::Error: return ImColor(255, 0, 0);
            default: return ImColor(255, 255, 255);
        }
    }();

    ImGui::TextColored(color, "%s", line.msg.c_str());

    ImGui::PushID(&line);

    if (ImGui::BeginPopupContextItem(line.msg.c_str()))
    {
        if (ImGui::Selectable("Copy"))
        {
            ImGui::SetClipboardText(line.msg.c_str());
        }

        ImGui::EndPopup();
    }

    ImGui::PopID();
}

void LoggerWindow::onLog(const std::string& msg, Logger::Level level)
{
    // don't waste memory
    if (_enabled)
    {
        _lines.emplace_back(Line{ msg, level });
    }
}

HRY_NS_END
