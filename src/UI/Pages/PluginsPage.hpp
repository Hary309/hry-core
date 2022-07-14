/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include "PageBase.hpp"

#include "Modules/Module.hpp"
#include "Modules/ModuleManager.hpp"

#include <memory>
#include <string>
#include <vector>

namespace hry
{
class PluginsPage : public PageBase
{
private:
    ModuleManager& _moduleMgr;

    const hry::PluginInfo* _selectedPlugin{};

public:
    PluginsPage(ModuleManager& moduleMgr);

    void imguiRender() override;

private:
    void renderList();
    void renderDetail();
};
}
