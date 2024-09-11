#pragma once
#include <functional>
#include <atomic>
#include <mutex>
#include <queue>
#include <thread>

namespace pe2d
{
    class TaskQueue
    {
    public:
        TaskQueue() = default;
    public:
        template<typename TCallback>
        void AddTask(TCallback &&callback)
        {
            std::lock_guard lck{m_Mutex};
            m_Tasks.push(std::forward(callback));
            m_RemainingTasks++;
        }
        void GetTask(std::function<void()> &targetCallback)
        {
            std::lock_guard lck{m_Mutex};
            if(m_Tasks.empty())
            {
                return;
            }
            targetCallback = std::move(m_Tasks.front());
            m_Tasks.pop();
        }
        static void Wait() { std::this_thread::yield(); }
        void WaitForCompletition() const
        {
            while(m_RemainingTasks > 0)
            {
                Wait();
            }
        }
        void WorkDone() { m_RemainingTasks--; }
    private:
        std::atomic<int> m_RemainingTasks{0};
        std::mutex m_Mutex;
        std::queue<std::function<void()>> m_Tasks;
    };
}