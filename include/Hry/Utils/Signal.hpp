/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

// original code: https://github.com/skypjack/entt
// This is a part of ENTT under MIT License

#pragma once

#include <vector>

#include "Hry/Namespace.hpp"

#include "Delegate.hpp"

HRY_NS_BEGIN

template<typename>
class Signal;

template<typename>
class Sink;

/**
 * @brief Signal handler
 * 
 * Used to notify multiple delegates
 *
 * @tparam Return Return type of a delegate function
 * @tparam Args Types of arguments of a delegate function
 */
template<typename Return, typename... Args>
class Signal<Return(Args...)> final
{
    friend Sink<Return(Args...)>;

private:
    using Delegate_t = Delegate<Return(Args...)>;

private:
    std::vector<Delegate_t> _calls;

public:
    /**
     * @brief Call the connected delegates
     * 
     * @param args Arguments to use to invoke the connected delegates
     */
    void call(Args... args) noexcept
    {
        if constexpr (sizeof...(args) > 0)
        {
            for (Delegate_t& delegate : _calls)
            {
                delegate.call(std::forward<Args>(args)...);
            }
        }
        else
        {
            for (Delegate_t& delegate : _calls)
            {
                delegate.call();
            }
        }
    }

private:
    /**
     * @brief Add delegate to invoke list
     * 
     * @param delegate Delegate to add
     */
    void add(Delegate_t delegate) noexcept { _calls.push_back(delegate); }

    /**
     * @brief Remove delegate from invoke list
     * 
     * @param delegate Delegate to remove
     */
    void remove(Delegate_t delegate) noexcept
    {
        auto it = std::find_if(_calls.begin(), _calls.end(), [&delegate](const Delegate_t& a) {
            return a == delegate;
        });

        if (it != _calls.end())
        {
            _calls.erase(it);
        }
    }
};

/**
 * @brief Sink
 * 
 * Sink is adaptor to signal class.
 * In destructor all connected delegates to sink all disconnected
 * 
 * @tparam Return Return type of a delegate function
 * @tparam Args Types of arguments of a delegate function
 */
template<typename Return, typename... Args>
class Sink<Return(Args...)> final
{
private:
    using Signal_t = Signal<Return(Args...)>;

private:
    Signal_t& _signal;
    std::vector<typename Signal_t::Delegate_t> _internalCalls;

public:
    /**
     * @brief Sink can be only constructed from signal class
     * 
     * @param signal The object that sink will be based on
     */
    Sink(Signal_t& signal) noexcept : _signal(signal) {}
    Sink(Sink&&) noexcept = default;
    Sink(const Sink&) noexcept = default;
    Sink& operator=(Sink&&) noexcept = default;
    Sink& operator=(const Sink&) noexcept = default;

    /**
     * @brief Disconnect all connected delegates
     */
    ~Sink() noexcept
    {
        for (auto& delegate : _internalCalls)
        {
            _signal.remove(delegate);
        }
    }

    /**
     * @brief Connects function to the delegate
     * 
     * @tparam FuncAddr Function to connect to the delegate
     */
    template<auto FuncAddr>
    void connect() noexcept
    {
        typename Signal_t::Delegate_t delegate;
        delegate.template connect<FuncAddr>();

        _signal.add(delegate);
        _internalCalls.push_back(delegate);
    }

    /**
    * @brief Connects function to the delegate with context
    * 
    * @tparam CtxFuncAddr Function with context to connect to the delegate
    * @tparam T Context type
    * @param content Pointer to content
    */
    template<auto MethodAddr, typename T>
    void connect(T* content) noexcept
    {
        typename Signal_t::Delegate_t delegate;
        delegate.template connect<MethodAddr>(content);

        _signal.add(delegate);
        _internalCalls.push_back(delegate);
    }

    /**
     * @brief Disconnect all connected delegates
     */
    void clear()
    {
        for (auto& delegate : _internalCalls)
        {
            _signal.remove(delegate);
        }

        _internalCalls.clear();
    }
};

template<typename Return, typename... Args>
Sink(Signal<Return(Args...)>&) -> Sink<Return(Args...)>;

HRY_NS_END
