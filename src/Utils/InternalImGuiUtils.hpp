/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once


namespace hry
{
struct InternalImGuiUtils
{
    // @enable: if true enable mouse input and show cursor
    static void EnableCursor(bool enable);

    static void ApplyDarkTheme();

    static void LoadFonts();
};
}
