#pragma once

#include <chrono>
#include <queue>
#include <tuple>
#include <type_traits>
#include <vector>

#include "Hry/Namespace.hpp"

#include "Delegate.hpp"

HRY_NS_BEGIN

template<typename>
class TaskScheduler;

template<typename Return, typename... Args>
class TaskScheduler<Return(Args...)>
{
public:
    using Delegate_t = Delegate<Return(Args...)>;
    using SystemClock_t = std::chrono::system_clock;
    using TimePoint_t = SystemClock_t::time_point;

private:
    struct Task
    {
        Delegate_t task;
        TimePoint_t timePoint;

        std::tuple<Args...> args;

        constexpr bool operator()(const Task& lhs, const Task& rhs) const
        {
            return lhs.timePoint > rhs.timePoint;
        }
    };

private:
    std::priority_queue<Task, std::vector<Task>, Task> _tasks;

public:
    void update()
    {
        while (!_tasks.empty() &&
               _tasks.top().timePoint - SystemClock_t::now() < TimePoint_t::duration::zero())
        {
            const auto& task = _tasks.top();
            invokeTask(task, task.args, std::make_index_sequence<sizeof...(Args)>());
            _tasks.pop();
        }
    }

    void addTask(std::chrono::milliseconds delay, Delegate_t&& delegate, Args... args)
    {
        _tasks.push({ delegate, SystemClock_t::now() + delay, std::tuple<Args...>(args...) });
    }

private:
    template<class Tuple, size_t... I>
    void invokeTask(const Task& task, Tuple& t, std::index_sequence<I...> /*unused*/)
    {
        task.task(std::get<I>(std::forward<Tuple>(t))...);
    }
};

HRY_NS_END