#pragma once

#include "Hry/Namespace.hpp"

HRY_NS_BEGIN

class PageBase
{
public:
    PageBase() = default;

    PageBase(PageBase&&) = delete;
    PageBase(const PageBase&) = delete;
    PageBase& operator=(PageBase&&) = delete;
    PageBase& operator=(const PageBase&) = delete;

    virtual ~PageBase() = default;

    virtual void imguiRender() = 0;
};

HRY_NS_END
