/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once


#include "Config/ConfigManager.hpp"

#include "PageBase.hpp"

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
