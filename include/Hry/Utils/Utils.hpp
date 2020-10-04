/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include <filesystem>
#include <memory>

#include <fmt/format.h>
#include <guiddef.h>

#include "Hry/Export.hpp"
#include "Hry/Namespace.hpp"
#include "Hry/Utils/Delegate.hpp"

HRY_NS_BEGIN

#ifdef UNICODE
#    define HRY_TEXT(text) L##text
#else
#    define HRY_TEXT(text) text
#endif // !UNICODE

template<typename T>
using HryPtr = std::unique_ptr<T, Delegate<void(T*)>>;

HRY_API std::string FormatGUID(const GUID& guid);

HRY_NS_END

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
            return format_to(
                ctx.out(), "{{{:x}-{:x}-{:x}-{:x}}}", guid.Data1, guid.Data2, guid.Data3,
                fmt::join(guid.Data4, ""));
        }
    };
} // namespace fmt
