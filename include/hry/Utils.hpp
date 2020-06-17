#pragma once

namespace hry
{

#ifdef UNICODE
#define HRY_TEXT(text) L##text
#else
#define HRY_TEXT(text) text
#endif // !UNICODE

}