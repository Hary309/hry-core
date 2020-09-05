#include "Hry/Utils.hpp"

#include <fmt/format.h>

#include "Hry/Namespace.hpp"

HRY_NS_BEGIN

std::string HRY_API FormatGUID(const GUID& guid)
{
    return fmt::format(
        "{{{:x}-{:x}-{:x}-{:x}}}", guid.Data1, guid.Data2, guid.Data3, fmt::join(guid.Data4, ""));
}

HRY_NS_END