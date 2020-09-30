/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#ifdef HRY_EXPORT
#    define HRY_API __declspec(dllexport)
#else
#    define HRY_API __declspec(dllimport)
#endif
