#pragma once

#include <vector>
#include <functional>

namespace hry
{

class Sink;

template<typename Func>
class Signal;

template<typename R, typename... Args>
class Signal<R(Args...)>
{
    friend Sink;

protected:
    std::vector<std::function<R(Args...)>> _delegates;

public:

protected:


};

class Sink
{
public:
    

};

}
