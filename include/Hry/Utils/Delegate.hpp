#pragma once

#include <functional>
#include <optional>
#include <type_traits>

namespace hry
{

template <typename>
class Delegate;

template <typename Return, typename... Args>
class Delegate<Return(Args...)>
{
private:
    using Delegate_t = Return(Args...);
    using Function_t = Return(void*, Args...);

private:
    Function_t* _function = nullptr;
    void* _content = nullptr;

public:
    Delegate() = default;

    template<Delegate_t* FuncAddr>
    void connect()
    {
        _function = [](void*, Args... args) -> Return {
            return static_cast<Return>(std::invoke(FuncAddr, std::forward<Args>(args)...));
        };

        _content = nullptr;
    }

    template<auto MethodAddr, typename T>
    void connect(T* content)
    {
        _function = [](void* content, Args... args) -> Return {
            return static_cast<Return>(std::invoke(MethodAddr, static_cast<T*>(content), std::forward<Args>(args)...));
        };

        _content = content;
    }

    template<
        typename Retn = std::conditional_t<
            std::is_void_v<Return>,
            void,
            std::optional<Return>
            >
        >
    Retn call(Args... args) const
    {
        if (_function)
        {
            return _function(_content, std::forward<Args>(args)...);
        }

        if constexpr (!std::is_void_v<Return>)
        {
            return {};
        }
    }
};

}

