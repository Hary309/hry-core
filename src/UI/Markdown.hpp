/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once
#include <string_view>

namespace hry
{
struct Markdown
{
    static void Render(std::string_view text);
};
}
