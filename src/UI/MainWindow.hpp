#pragma once

#include "Modules/ModuleManager.hpp"

namespace hry::ui
{

class MainWindow
{
private:
    modules::ModuleManager& _moduleMgr;

    int _selectedPluginIndex = 0;

public:
    MainWindow(modules::ModuleManager& _moduleMgr);

    void renderImGui();

private:
    void renderPluginsTab();
    void renderSettingsTab();
    void renderPluginsSettingsTab();
    void renderAboutTab();
};

}
