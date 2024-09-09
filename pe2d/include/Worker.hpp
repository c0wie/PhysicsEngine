#pragma once
#include "TaskQueue.hpp"

namespace pe2d
{
    class Worker
    {
    public:
        Worker() = default;
        Worker(TaskQueue& queue, int id) :
            m_ID(id), m_Queue(&queue)
        {
            m_Thread = std::thread([this](){
                Run();
            });
        }

        void Run()
        {
            while(m_IsRunning)
            {
                m_Queue->GetTask(m_Task);
                if(!m_Task)
                {
                    TaskQueue::Wait();
                }
                else
                {
                    m_Task();
                    m_Queue->WorkDone();
                    m_Task = nullptr;
                }
            }
        }

        void Stop()
        {
            m_IsRunning = false;
            m_Thread.join();
        }
        
    private:
        std::thread m_Thread;
        std::function<void()> m_Task{nullptr};
        TaskQueue* m_Queue{nullptr};
        int m_ID {0};
        bool m_IsRunning{true};
    };
    
}