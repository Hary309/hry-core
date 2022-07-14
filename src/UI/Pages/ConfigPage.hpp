/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include "PageBase.hpp"

#include "Config/ConfigManager.hpp"

namespace hry
{
class ConfigPage : public PageBase
{
private:
    ConfigManager& _configMgr;

public:
    explicit ConfigPage(ConfigManager& configMgr);

    void imguiRender() override;
};
}
