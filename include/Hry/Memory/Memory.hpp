/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include <cstdint>
#include <memory>

#include <Windows.h>


namespace hry
{
/**
 * @brief Write data to any memory address with specified size
 *
 * Function unprotects memory, so it writes to any address
 *
 * @tparam T Target type
 * @tparam U Data type
 * @param target Pointer to data, which will be filled with \p data
 * @param data Data to fill pointer
 * @param size Size of data
 */
template<typename T, typename U>
void WriteMemory(T* target, U data, const size_t size)
{
    DWORD oldProtect = 0;
    DWORD newProtect = 0;

    VirtualProtect(reinterpret_cast<void*>(target), size, PAGE_READWRITE, &oldProtect);

    memcpy(reinterpret_cast<void*>(target), &data, size);

    VirtualProtect(reinterpret_cast<void*>(target), size, oldProtect, &newProtect);
}

/**
 * @brief Write data to any memory address
 *
 * @tparam T Target type
 * @tparam U Data type
 * @param target Pointer to data, which will be filled with \p data
 * @param data Data to fill pointer
 */
template<typename T, typename U>
void WriteMemory(T* target, U data)
{
    WriteMemory(target, data, sizeof(U));
}

/**
 * @brief Get the base address
 *
 * Useful for fixed pointer but not recommended!
 *
 * @return Base address of game
 */
inline uintptr_t GetBaseAddress()
{
    return reinterpret_cast<uintptr_t>(GetModuleHandle(nullptr)) - 0x140000000;
}
}
