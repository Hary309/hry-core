/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include "Hry/Namespace.hpp"

#include "Config/ConfigManager.hpp"

#include "PageBase.hpp"

HRY_NS_BEGIN

class ConfigPage : public PageBase
{
private:
    ConfigManager& _configMgr;

public:
    explicit ConfigPage(ConfigManager& configMgr);

    void imguiRender() override;
};

HRY_NS_END
