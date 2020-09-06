#pragma once

#include "Hry/Namespace.hpp"

HRY_NS_BEGIN

struct InternalImGuiUtils
{
    // @enable: if true enable mouse input and show cursor
    static void EnableCursor(bool enable);

    static void ApplyDarkTheme();

    static void LoadFonts();
};

HRY_NS_END
