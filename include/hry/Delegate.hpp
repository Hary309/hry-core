#pragma once

#include <functional>
#include <optional>

namespace hry
{

template <typename>
class Delegate;

template <typename Return, typename... Args>
class Delegate<Return(Args...)>
{
private:
    using Delegate_t = Return(Args...);
    using Function_t = Return(const void*, Args...);

private:
    Function_t* _function = nullptr;
    const void* _content = nullptr;

public:
    Delegate() = default;

    template<Delegate_t* FuncAddr>
    void connect()
    {
        _function = [](const void*, Args... args) -> Return {
            return static_cast<Return>(std::invoke(FuncAddr, std::forward<Args>(args)...));
        };

        _content = nullptr;
    }

    template<auto MethodAddr, typename T>
    void connect(const T* content)
    {
        _function = [](const void* content, Args... args) -> Return {
            return static_cast<Return>(std::invoke(MethodAddr, static_cast<const T*>(content), std::forward<Args>(args)...));
        };

        _content = content;
    }

    std::optional<Return> call(Args... args) const
    {
        if (_function == nullptr)
        {
            return {};
        }

        return _function(_content, std::forward<Args>(args)...);
    }
};

}

