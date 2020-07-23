#pragma once

#include "Hry/Namespace.hpp"

HRY_NS_BEGIN

#ifdef UNICODE
#    define HRY_TEXT(text) L##text
#else
#    define HRY_TEXT(text) text
#endif // !UNICODE

HRY_NS_END