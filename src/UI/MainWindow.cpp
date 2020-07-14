#include "MainWindow.hpp"
#include "imgui.h"

namespace hry::ui
{

MainWindow::MainWindow(modules::ModuleManager& moduleMgr)
    : _moduleMgr(moduleMgr)
{

}

void MainWindow::renderImGui() 
{
    ImGui::SetNextWindowSize({400.f, 300.f}, ImGuiCond_FirstUseEver);
    
    if (ImGui::Begin("Plugin Manager"))
    {
        if (ImGui::BeginTabBar("MainWindow::Tab"))
        {
            if (ImGui::BeginTabItem("Plugins"))
            {
                renderPluginsTab();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Settings"))
            {
                renderSettingsTab();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Plugins settings"))
            {
                renderPluginsSettingsTab();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("About"))
            {
                renderAboutTab();
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
    }

    ImGui::End();
}


void MainWindow::renderPluginsTab() 
{
    for (auto& module : _moduleMgr.getModules())
    {
        if (module->isLoaded)
        {
            auto& pluginInfo = module->plugin->getPluginInfo();
            if (ImGui::SmallButton("Unload"))
            {
                _moduleMgr.unload(module.get());
            }
            else
            {
                ImGui::SameLine();
                ImGui::BulletText("%s - %s", pluginInfo.shortName.c_str(), pluginInfo.shortDesc.c_str());
            }
        }
        else
        {
            if (ImGui::SmallButton("Load"))
            {
                _moduleMgr.load(module.get());
            }
            else
            {
                ImGui::SameLine();
                ImGui::BulletText("%s", module->dllName.c_str());
            }
        }
    }
}


void MainWindow::renderSettingsTab() 
{
    ImGui::Text("Settings related to this plugin, also some developer options");
}


void MainWindow::renderPluginsSettingsTab() 
{
    ImGui::Text("Settings related to loaded plugins");
}


void MainWindow::renderAboutTab() 
{
    ImGui::Text("Credits etc");
}


}
