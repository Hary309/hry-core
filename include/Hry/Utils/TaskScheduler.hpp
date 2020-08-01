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
    using TaskID_t = uint64_t;

    struct Task
    {
        Delegate_t task;
        TimePoint_t timePoint;

        std::tuple<Args...> args;

        uint64_t id = 0;

        bool enabled = true;

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
        // remove disabled tasks
        while (!_tasks.top().enabled)
        {
            _tasks.pop();
        }

        while (_tasks.top().timePoint - SystemClock_t::now() < std::chrono::milliseconds(0))
        {
            const auto& task = _tasks.top();
            invokeTask(task, task.args, std::make_index_sequence<sizeof...(Args)>());
            _tasks.pop();
        }
    }

    void disableTask(TaskID_t id)
    {
        auto it = std::find_if(std::begin(_tasks), std::end(_tasks), [&id](const Task& task) {
            return task.id == id;
        });

        if (it != std::end(_tasks))
        {
            it->enabled = false;
        }
    }

    void hasTask(TaskID_t id)
    {
        auto it = std::find_if(std::begin(_tasks), std::end(_tasks), [&id](const Task& task) {
            return task.id == id;
        });

        return it != std::end(_tasks);
    }

    TaskID_t addTask(std::chrono::milliseconds delay, Delegate_t&& delegate, Args... args)
    {
        Task task{ delegate, SystemClock_t::now() + delay, std::tuple<Args...>(args...) };

        auto id = reinterpret_cast<TaskID_t>(&task);
        task.id = id;
        _tasks.push(std::move(task));

        return id;
    }

private:
    template<class Tuple, size_t... I>
    void invokeTask(const Task& task, Tuple& t, std::index_sequence<I...> /*unused*/)
    {
        task.task(std::get<I>(std::forward<Tuple>(t))...);
    }
};

HRY_NS_END