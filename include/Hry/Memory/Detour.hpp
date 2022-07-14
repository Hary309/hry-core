/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include "Hry/Export.hpp"

#include <cstdint>
#include <type_traits>

namespace hry
{
/**
 * @brief Class for creating hooks
 */
class HRY_API Detour
{
public:
    enum class Status
    {
        Unknown = -1,
        /** @brief Successful */
        Ok = 0,
        /** @brief Hook already initialized */
        AlreadyInitialized,
        /** @brief Hook not initialized yet, or already uninitialized */
        NotInitialized,
        /** @brief The hook for the specified target function is already created */
        AlreadyCreated,
        /** @brief The hook for the specified target function is not created yet */
        NotCreated,
        /** @brief The hook for the specified target function is already enabled */
        AlreadyEnabled,
        /** @brief The hook for the specified target function is not enabled yet, or already disabled */
        Disabled,
        /** @brief The specified pointer is invalid. It points the address of non-allocated and/or non-executable region */
        NotExecutable,
        /** @brief The specified target function cannot be hooked */
        UnsupportedFunction,
        /** @brief Failed to allocate memory */
        MemoryAlloc,
        /** @brief Failed to change the memory protection */
        MemoryProtect,
        /** @brief The specified module is not loaded */
        ModuleNotFound,
        /** @brief The specified function is not found */
        FunctionNotFound
    };

private:
    uintptr_t* _target{};
    uintptr_t* _detour{};
    uintptr_t* _original{};

public:
    /**
     * @brief Construct a new Detour
     *
     * @param target A pointer to the target function, which will be
                     overridden by the detour function.
     * @param detour A pointer to the detour function, which will override
                     the target function.
     */
    Detour(uintptr_t target, uintptr_t detour);

    /**
     * @brief Construct a new Detour
     *
     * @param target A pointer to the target function, which will be
                     overridden by the detour function.
     * @param detour A pointer to the detour function, which will override
                     the target function.
     */
    Detour(uintptr_t* target, uintptr_t* detour);

    /**
     * @brief Construct a new Detour object
     *
     * @tparam Function Function type
     * @param target A pointer to the target function, which will be
                     overridden by the detour function.
     * @param detour A pointer to the detour function, which will override
                     the target function.
     */
    template<typename Function>
    Detour(Function target, Function detour)
        : _target(reinterpret_cast<uintptr_t*>(target))
        , _detour(reinterpret_cast<uintptr_t*>(detour))
    {
    }

    Detour(Detour&&) = delete;
    Detour(const Detour&) = delete;
    Detour& operator=(Detour&&) = delete;
    Detour& operator=(const Detour&) = delete;
    ~Detour();

    /**
     * @brief Creates a Hook
     *
     * @return Error code
     */
    Status create();
    /**
     * @brief Removes an already created hook
     *
     * @return Error code
     */
    Status remove();

    /**
     * @brief Enables an already created hook
     *
     * @return Error code
     */
    Status enable();

    /**
     * @brief Disables an already created hook
     *
     * @return Error code
     */
    Status disable();

    /**
     * @brief Cast original function to provided type
     *
     * @tparam T Function type
     * @return Pointer to function with specified type
     */
    template<typename T>
    std::remove_pointer_t<T>* getOriginal()
    {
        return reinterpret_cast<std::remove_pointer_t<T>*>(_original);
    }
};
}
