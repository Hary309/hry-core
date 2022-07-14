/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#include "Hry/Utils/Utils.hpp"

namespace hry
{
std::string HRY_API FormatGUID(const GUID& guid)
{
    return fmt::format(
        "{{{:x}-{:x}-{:x}-{:x}}}", guid.Data1, guid.Data2, guid.Data3, fmt::join(guid.Data4, ""));
}
}