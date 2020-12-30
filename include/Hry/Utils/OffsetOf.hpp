/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include "Hry/Namespace.hpp"

HRY_NS_BEGIN

/**
 * @brief Deduces offset of member in class
 * 
 * @param member pointer to class' member
 * @return Offset of member
 */
template <typename T, typename Class>
size_t OffsetOf(T Class::* member)
{
    return reinterpret_cast<size_t>(&(reinterpret_cast<Class*>(0)->*member));
}

HRY_NS_END
