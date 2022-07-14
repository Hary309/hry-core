/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

// original code: https://github.com/skypjack/entt
// This is a part of ENTT under MIT License

#pragma once

#include <functional>
#include <optional>
#include <type_traits>


namespace hry
{
template<typename>
class Delegate;

template<auto>
struct ConnectArg
{
};

template<auto Addr>
inline static ConnectArg<Addr> ConnectArg_v;

/**
 * @brief Utility class to sned information around the hry-core
 *        Used for events system
 *
 * @tparam Return Return type of a delegate function
 * @tparam Args Types of arguments of a delegate function
 */
template<typename Return, typename... Args>
class Delegate<Return(Args...)> final
{
public:
    /**
     * @brief Internal function type
     */
    using Function_t = Return(void*, Args...);

private:
    Function_t* _function = nullptr;
    void* _content = nullptr;

public:
    Delegate() = default;

    /**
     * @brief Constructs delegate and connect function
     *
     * @tparam FuncAddr function to connect to the delegate
     */
    template<auto FuncAddr>
    Delegate(ConnectArg<FuncAddr> /*unused*/) noexcept
    {
        connect<FuncAddr>();
    }

    /**
     * @brief Constructs delegate with context and connect function
     *
     * @tparam CtxFuncAddr function to connect to the delegate
     * @tparam T context type
     */
    template<auto CtxFuncAddr, typename T>
    Delegate(ConnectArg<CtxFuncAddr> /*unused*/, T* context) noexcept
    {
        connect<CtxFuncAddr>(context);
    }

    /**
     * @brief Constructs delegate with raw function pointer and context
     *
     * @param func Function to connect to the delegate
     * @param context Pointer to content
     */
    Delegate(Function_t* func, void* context) noexcept { connect(func, context); }

    /**
     * @brief Connects function to the delegate
     *
     * @tparam FuncAddr Function to connect to the delegate
     */
    template<auto FuncAddr>
    void connect() noexcept
    {
        static_assert(
            std::is_invocable_r_v<Return, decltype(FuncAddr), Args...>,
            "Passed function doesn't meet declared function template");

        _function = [](void* /*unused*/, Args... args) -> Return {
            return static_cast<Return>(std::invoke(FuncAddr, std::forward<Args>(args)...));
        };

        _content = nullptr;
    }

    /**
    * @brief Connects function to the delegate with context
    *
    * @tparam CtxFuncAddr Function with context to connect to the delegate
    * @tparam T Context type
    * @param content Pointer to content
    */
    template<auto CtxFuncAddr, typename T>
    void connect(T* content) noexcept
    {
        static_assert(
            std::is_invocable_r_v<Return, decltype(CtxFuncAddr), T*, Args...>,
            "Passed method doesn't meet declared method template");

        _function = [](void* content, Args... args) -> Return {
            return static_cast<Return>(
                std::invoke(CtxFuncAddr, static_cast<T*>(content), std::forward<Args>(args)...));
        };

        _content = content;
    }

    /**
     * @brief Connects raw function with context to the delegate
     *
     * @param func function to connect to the delegate
     * @param context Pointer to content
     */
    void connect(Function_t* func, void* context) noexcept
    {
        _function = func;
        _content = context;
    }

    /**
     * @brief Call the connected function
     *
     * @param args Arguments to use to invoke the connected function
     * @return Value returned by the connected function
     */
    Return call(Args... args) const
    {
        if (_function)
        {
            return _function(_content, std::forward<Args>(args)...);
        }
    }

    /**
     * @brief Reset the delegate
     */
    void reset() noexcept
    {
        _function = nullptr;
        _content = nullptr;
    }

    /**
     * @brief Call the connected function
     *
     * @param args Arguments to use to invoke the connected function
     * @return Value returned by the connected function
     */
    Return operator()(Args... args) const { return call(std::forward<Args>(args)...); }

    /**
     * @brief Compare the content of two delegates
     *
     * @param other Other delegate to compare
     * @return false if the fields inside are different
     */
    bool operator==(const Delegate<Return(Args...)>& other) const noexcept
    {
        return (_function == other._function && _content == other._content);
    }
};

template<typename Return, typename... Args>
auto FunctionPtr(Return (*)(Args...)) -> Return (*)(Args...);

template<typename Class, typename Return, typename... Args, typename... Other>
auto FunctionPtr(Return (Class::*)(Args...), Other&&...) -> Return (*)(Args...);

template<typename Class, typename Return, typename... Args, typename... Other>
auto FunctionPtr(Return (Class::*)(Args...) const, Other&&...) -> Return (*)(Args...);

template<typename... Types>
using FunctionPtr_t = decltype(FunctionPtr(std::declval<Types>()...));

template<auto FuncAddr>
Delegate(ConnectArg<FuncAddr>)
    -> Delegate<std::remove_pointer_t<FunctionPtr_t<decltype(FuncAddr)>>>;

template<auto CtxFuncAddr, typename T>
Delegate(ConnectArg<CtxFuncAddr>, T* context)
    -> Delegate<std::remove_pointer_t<FunctionPtr_t<decltype(CtxFuncAddr), T>>>;

/**
 * @brief Inline constructs delegate
 *
 * @tparam FuncAddr Function to connect to the delegate
 * @return Constructed delegate
 */
template<auto FuncAddr>
auto Dlg() noexcept
{
    return Delegate<std::remove_pointer_t<FunctionPtr_t<decltype(FuncAddr)>>>{
        ConnectArg_v<FuncAddr>
    };
}

/**
 * @brief Inline constructs delegate with context
 *
 * @tparam CtxFuncAddr Function with context to connect to the delegate
 * @tparam T Context type
 * @param content Context data
 * @return Constructed delegate
 */
template<auto CtxFuncAddr, typename T>
auto Dlg(T* content) noexcept
{
    return Delegate<std::remove_pointer_t<FunctionPtr_t<decltype(CtxFuncAddr), T>>>{
        ConnectArg_v<CtxFuncAddr>, content
    };
}
}
