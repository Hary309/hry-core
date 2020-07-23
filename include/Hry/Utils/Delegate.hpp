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
{};

template<auto Addr>
inline constexpr ConnectArg<Addr> ConnectArg_v;

template<typename Return, typename... Args>
class Delegate<Return(Args...)>
{
public:
    using Function_t = Return(void*, Args...);

private:
    Function_t* _function = nullptr;
    void* _content = nullptr;

public:
    Delegate() = default;

    template<auto FuncAddr>
    Delegate(ConnectArg<FuncAddr>)
    {
        connect<FuncAddr>();
    }

    template<auto CtxFuncAddr, typename T>
    Delegate(ConnectArg<CtxFuncAddr>, T* context)
    {
        connect<CtxFuncAddr>(context);
    }

    template<auto FuncAddr>
    void connect()
    {
        static_assert(
            std::is_invocable_r_v<Return, decltype(FuncAddr), Args...>,
            "Passed function doesn't meet declared function template");

        _function = [](void*, Args... args) -> Return {
            return static_cast<Return>(std::invoke(FuncAddr, std::forward<Args>(args)...));
        };

        _content = nullptr;
    }

    template<auto CtxFuncAddr, typename T>
    void connect(T* content)
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

    template<auto FuncAddr>
    void connect(ConnectArg<FuncAddr>)
    {
        connect<FuncAddr>();
    }

    template<auto CtxFuncAddr, typename T>
    void connect(ConnectArg<CtxFuncAddr>, T* context)
    {
        connect<CtxFuncAddr>(context);
    }

    void connect(Function_t* func, void* context)
    {
        _function = func;
        _content = context;
    }

    Return call(Args... args) const
    {
        if (_function)
        {
            return _function(_content, std::forward<Args>(args)...);
        }
    }

    Return operator()(Args... args) const { return call(std::forward<Args>(args)...); }

    bool operator==(const Delegate<Return(Args...)>& b) const
    {
        return (_function == b._function && _content == b._content);
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

} // namespace hry
