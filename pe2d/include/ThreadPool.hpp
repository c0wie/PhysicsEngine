#pragma once
#include "Worker.hpp"

namespace pe2d
{
    class ThreadPool
    {
    public:
        explicit ThreadPool(int threadCount) :
            m_ThreadCount(threadCount)
        {
            m_Workers.reserve(threadCount);
            for(int i = threadCount; i >= 0; i--)
            {
                m_Workers.emplace_back(m_Queue, m_Workers.size());
            }
        }
        ThreadPool(const ThreadPool &other) = default;
        ThreadPool& operator=(const ThreadPool  &other) = default;
        ThreadPool(ThreadPool &&other) = default;
        ThreadPool& operator=(ThreadPool &&other) = default;
        ~ThreadPool()
        {
            for(auto &worker : m_Workers)
            {
                worker.Stop();
            }
        }
    public:
        template<typename TCallback>
        void AddTask(TCallback &callback)
        {
            m_Queue.AddTask(std::forward(callback));
        }

        void WaitForCompletion() const
        {
            m_Queue.WaitForCompletition();
        }

        template<typename TCallback>
        void dispatch(int elementCount, TCallback && callback)
        {
            const int batchSize = elementCount / m_ThreadCount;
            for(int i = 0; i < m_ThreadCount; i++)
            {
                const int start = batchSize * i;
                const int end = start + batchSize;
                AddTask([start, end, callback](){ callback(start, end, callback)}); 
            }

            if(batchSize * m_ThreadCount < elementCount)
            {
                const int start = batchSize * m_ThreadCount;
                callback(start, elementCount);
            }

            WaitForCompletion();
        }
    private:
        int m_ThreadCount{0};
        TaskQueue m_Queue;
        std::vector<Worker> m_Workers;
    };
}