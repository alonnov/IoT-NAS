/********************************
 Name:     Alon Nov
 Exercise: thread pool
 Date:	   10.03.2025   
 Reviewer: Itai Hirschhorn

********************************/

#include "ThreadPool.hpp"   // API, std::jthread, std::stop_token, acquire, release, std::hardware_concurrency, std::make_shared

using namespace ilrd::details;

///////////////////////////ThreadPool///////////////////////////////
ThreadPool::ThreadPool(size_t num_threads) : m_sem(0), m_paused(false)
{   
    m_kill_task = std::make_shared<KillTask>(this);
    m_pause_task = std::make_shared<PauseTask>(this);
    
    while(num_threads--)
    {
        AddThread();
    }
}

void ThreadPool::ThreadFunc(std::stop_token token)
{
    PriorityTask out(NULL, MED);

    while(!token.stop_requested())
    {
        m_waitq.Pop(out);        
        out.Run();
    }
} 


ThreadPool::~ThreadPool()
{
    Resume();
    size_t size = m_threads.size();

    while(size--)
    {
        RemoveThread();
    }
}


void ThreadPool::AddTask(std::shared_ptr<ITPTask> task, PRIORITY priority)
{
    m_waitq.Push(PriorityTask(task, priority));
}


void ThreadPool::SetNumThreads(size_t num_threads)
{
    size_t size = m_threads.size();

    if(size < num_threads)
    {
        while(size < num_threads)
        {
            AddThread();
            ++size;
        }
    }  
    else
    {
        while(num_threads--)
        {
            RemoveThread();  
        }
    }
}


void ThreadPool::Pause()
{
    size_t size = m_threads.size();

    while(size--)
    {
        AddTask(m_pause_task, static_cast<PRIORITY>(SYS_HIGHEST));
    }

    m_paused = true;
}


void ThreadPool::Resume()
{
    if(m_paused)
    {
        size_t size = m_threads.size();

        while(size--)
        {
            m_sem.release();
        }
    }    
}

void ThreadPool::AddThread()
{
    std::unique_ptr<std::jthread> thread = std::make_unique<std::jthread>(
        [this](std::stop_token token)
            {
                ThreadFunc(token);
            });

    m_threads.emplace(thread->get_id(), std::move(thread));
}


void ThreadPool::RemoveThread()
{
    AddTask(m_kill_task, static_cast<PRIORITY>(SYS_HIGHEST));
    
    std::jthread::id tid;
    m_removed.Pop(tid);
    m_threads.erase(tid);
}


///////////////////////////PriorityTask///////////////////////////////
ThreadPool::PriorityTask::PriorityTask(std::shared_ptr<ITPTask> task_, PRIORITY priority_) : 
m_task(task_), m_priority(priority_), m_timestamp(std::chrono::high_resolution_clock::now())
{
}


// ThreadPool::PriorityTask::PriorityTask(const ThreadPool::PriorityTask& other_) : 
// m_task(other_.m_task), m_priority(other_.m_priority), m_timestamp(other_.m_timestamp)
// {
// }


// const ThreadPool::PriorityTask& ThreadPool::PriorityTask::operator=(const ThreadPool::PriorityTask& other_)
// {
//     m_task = other_.m_task;
//     m_priority = other_.m_priority;
//     m_timestamp = other_.m_timestamp;

//     return *this;
// }


bool ThreadPool::PriorityTask::operator<(const PriorityTask& other_) const
{
    return m_priority < other_.m_priority && m_timestamp > other_.m_timestamp;
}

void ThreadPool::PriorityTask::Run()
{
    m_task->Run();
}


///////////////////////////KillTask/////////////////////////////////////
ThreadPool::KillTask::KillTask(ThreadPool* tp_) : m_tp(tp_)
{
}


void ThreadPool::KillTask::Run()
{
    std::jthread::id tid = std::this_thread::get_id();
    
    std::unique_ptr<std::jthread>& thread = m_tp->m_threads.at(tid);
    thread->request_stop();
    m_tp->m_removed.Push(tid);
}

///////////////////////////PauseTask/////////////////////////////////////
ThreadPool::PauseTask::PauseTask(ThreadPool* tp_) : m_tp(tp_)
{
}


void ThreadPool::PauseTask::Run()
{
    m_tp->m_sem.acquire();
}