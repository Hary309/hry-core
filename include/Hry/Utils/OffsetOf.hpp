#pragma once

#include "Hry/Namespace.hpp"

HRY_NS_BEGIN

template <typename T, typename Class>
size_t OffsetOf(T Class::* member)
{
    return reinterpret_cast<size_t>(&(reinterpret_cast<Class*>(0)->*member));
}

HRY_NS_END
