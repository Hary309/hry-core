#pragma once

#include <memory>

#include "Hry/Namespace.hpp"
#include "Hry/Utils/Delegate.hpp"

HRY_NS_BEGIN

#ifdef UNICODE
#    define HRY_TEXT(text) L##text
#else
#    define HRY_TEXT(text) text
#endif // !UNICODE

template<typename T>
using DelegateDeleterUniquePtr_t = std::unique_ptr<T, Delegate<void(T*)>>;

HRY_NS_END