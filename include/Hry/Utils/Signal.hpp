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

template<typename Return, typename... Args>
class Signal<Return(Args...)>
{
    friend Sink<Return(Args...)>;

private:
    using Delegate_t = Delegate<Return(Args...)>;

private:
    std::vector<Delegate_t> _calls;

public:
    Return call(Args... args)
    {
        if constexpr (sizeof...(args) > 0)
        {
            for (Delegate_t& delegate : _calls)
            {
                delegate.call(std::forward<Args...>(args...));
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
    void add(Delegate_t delegate) { _calls.push_back(delegate); }

    void remove(Delegate_t delegate)
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

template<typename Return, typename... Args>
class Sink<Return(Args...)>
{
private:
    using Signal_t = Signal<Return(Args...)>;

private:
    Signal_t& _signal;
    std::vector<typename Signal_t::Delegate_t> _internalCalls;

public:
    Sink(Signal_t& signal) : _signal(signal) {}
    Sink(Sink&&) noexcept = default;
    Sink(const Sink&) = default;
    Sink& operator=(Sink&&) noexcept = default;
    Sink& operator=(const Sink&) = default;

    ~Sink()
    {
        for (auto& delegate : _internalCalls)
        {
            _signal.remove(delegate);
        }
    }

    template<auto FuncAddr>
    void connect()
    {
        typename Signal_t::Delegate_t delegate;
        delegate.template connect<FuncAddr>();

        _signal.add(delegate);
        _internalCalls.push_back(delegate);
    }

    template<auto MethodAddr, typename T>
    void connect(T* content)
    {
        typename Signal_t::Delegate_t delegate;
        delegate.template connect<MethodAddr>(content);

        _signal.add(delegate);
        _internalCalls.push_back(delegate);
    }
};

template<typename Return, typename... Args>
Sink(Signal<Return(Args...)>&) -> Sink<Return(Args...)>;

HRY_NS_END
