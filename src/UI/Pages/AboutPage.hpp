/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include "Hry/Namespace.hpp"

#include "UI/Pages/PageBase.hpp"

HRY_NS_BEGIN

class AboutPage : public PageBase
{
public:
    void imguiRender() override;
};

HRY_NS_END
