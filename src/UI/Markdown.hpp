#pragma once
#include <string_view>

#include "Hry/Namespace.hpp"

HRY_NS_BEGIN

struct Markdown
{
    static void Render(std::string_view text);
};

HRY_NS_END
