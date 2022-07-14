/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once


namespace hry
{
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
}
