#pragma once

#include <Windows.h>
#include <cstdint>
#include <memory>

namespace hry
{
template<typename T, typename U>
void WriteMemory(T* target, U data, const size_t size)
{
    DWORD oldProtect, newProtect;

    VirtualProtect(reinterpret_cast<void*>(target), size, PAGE_READWRITE, &oldProtect);

    memcpy(reinterpret_cast<void*>(target), &data, size);

    VirtualProtect(reinterpret_cast<void*>(target), size, oldProtect, &newProtect);
}

template<typename T, typename U>
void WriteMemory(T* target, U data)
{
    WriteMemory(target, data, sizeof(U));
}

} // namespace hry
