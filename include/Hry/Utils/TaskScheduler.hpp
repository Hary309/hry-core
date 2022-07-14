/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include "Delegate.hpp"

#include <chrono>
#include <queue>
#include <tuple>
#include <type_traits>
#include <vector>

namespace hry
{
template<typename>
class TaskScheduler;

/**
 * @brief Schedule task by time
 *
 * @tparam Return Return type of a delegate function
 * @tparam Args Types of arguments of a delegate function
 */
template<typename Return, typename... Args>
class TaskScheduler<Return(Args...)>
{
public:
    using Delegate_t = Delegate<Return(Args...)>;
    using SystemClock_t = std::chrono::system_clock;
    using TimePoint_t = SystemClock_t::time_point;

private:
    /**
     * @brief Base of task object, used in queue
     */
    struct Task
    {
        Delegate_t task;
        TimePoint_t timePoint;

        std::tuple<Args...> args;

        /**
         * @brief Used for sorting
         */
        constexpr bool operator()(const Task& lhs, const Task& rhs) const
        {
            return lhs.timePoint > rhs.timePoint;
        }
    };

private:
    std::priority_queue<Task, std::vector<Task>, Task> _tasks;

public:
    /**
     * @brief Update queue and invoke ready to invoke tasks
     */
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

    /**
     * @brief Add task to scheduler
     *
     * @param delay In what time task should be called
     * @param delegate Task to be invoked
     * @param args Arguments for delegate invoke
     */
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
}