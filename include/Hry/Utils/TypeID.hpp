/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#include "Hash.hpp"

HRY_NS_BEGIN

template<typename T>
constexpr Hash64_t TypeID() noexcept
{
#if defined(_MSC_VER) && !defined(__clang__)
    return FNV1a_64(__FUNCSIG__);
#else
    return FNV1a_64(__PRETTY_FUNCTION__);
#endif
}

HRY_NS_END