/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include "Hry/Export.hpp"
#include "Hry/Utils/Delegate.hpp"

#include <fmt/format.h>
#include <guiddef.h>

#include <filesystem>
#include <memory>

namespace hry
{
#ifdef UNICODE
#    define HRY_TEXT(text) L##text
#else
#    define HRY_TEXT(text) text
#endif // !UNICODE

/**
 * @brief Unique pointer with delegate destructor
 *
 * @tparam T Type to be hold
 */
template<typename T>
using HryPtr = std::unique_ptr<T, Delegate<void(T*)>>;
}

namespace fmt
{
template<>
struct formatter<GUID>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx)
    {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(GUID const& guid, FormatContext& ctx)
    {
        return format_to(ctx.out(), "{{{:08x}-{:04x}-{:04x}-{:02x}}}", guid.Data1, guid.Data2, guid.Data3, fmt::join(guid.Data4, ""));
    }
};
}
