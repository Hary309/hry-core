#pragma once

#ifdef HRY_EXPORT
   #define HRY_API __declspec(dllexport)
#else
   #define HRY_API __declspec(dllimport)
#endif
