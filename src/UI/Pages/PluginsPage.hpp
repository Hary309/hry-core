/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Modules/Module.hpp"
#include "Modules/ModuleManager.hpp"

#include "PageBase.hpp"

HRY_NS_BEGIN

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

HRY_NS_END
