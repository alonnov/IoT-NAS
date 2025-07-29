/********************************
 Name:     Alon Nov
 Exercise: thread pool
 Date:	   10.03.2025   
 Reviewer: Itai Hirschhorn

********************************/

#ifndef ILRD_RD1645_THREADPOOL_HPP
#define ILRD_RD1645_THREADPOOL_HPP

#include <unordered_map>// std::unordered_map
#include <thread>       // std::jthread, std::stop_token
#include <memory>

#include "ITPTask.hpp"
#include "WaitQ.hpp"
#include "PQ.hpp"

namespace ilrd
{    
namespace details
{


class ThreadPool
{
public:

    // task priorities
    enum PRIORITY
    {
        LOW = 1,
        MED = 2,
        HIGH = 3
    };

    // creates a thread pool with a given number of threads, 
    // default argument is number of cores on system (-1 for main thread)
    ThreadPool(size_t  = std::jthread::hardware_concurrency() - 1);

    // destroys the thread pool
    ~ThreadPool();

    // adds a task for a thread from the pool to run
    void AddTask(std::shared_ptr<ITPTask>, PRIORITY = MED);

    // changes the number of running threads in the pool, 
    // default argument is number of cores on system (-1 for main thread)
    void SetNumThreads(size_t  = std::jthread::hardware_concurrency() - 1);

    // pauses all threads
    void Pause();

    // resumes all paused threads
    void Resume();

private:
    class PriorityTask
    {
    public:
        explicit PriorityTask(std::shared_ptr<ITPTask>, PRIORITY);
        void Run();
        // PriorityTask(const PriorityTask& other_);
        // const PriorityTask& operator=(const PriorityTask& other_);
        bool operator<(const PriorityTask& other_) const;

    private:
        std::shared_ptr<ITPTask> m_task;
        PRIORITY m_priority;
        std::chrono::high_resolution_clock::time_point m_timestamp;
    };

    class KillTask : public ITPTask
    {
    public:
        KillTask(ThreadPool* tp_);
        void Run();
        ThreadPool* m_tp;
    };

    class PauseTask : public ITPTask
    {
    public:
        PauseTask(ThreadPool* tp_);
        void Run();
        ThreadPool* m_tp;
    };

    enum SYS_PRIORITY
    {
        SYS_LOW = 1,
        SYS_MED = 2,
        SYS_HIGH = 3,
        SYS_HIGHEST = 4
    };

    void ThreadFunc(std::stop_token);
    void AddThread();
    void RemoveThread();
    std::shared_ptr<KillTask> m_kill_task;
    std::shared_ptr<PauseTask> m_pause_task;
    std::unordered_map<std::jthread::id, std::unique_ptr<std::jthread>> m_threads;
    WaitQ<PriorityTask, PQ<PriorityTask>> m_waitq;
    WaitQ<std::jthread::id, PQ<std::jthread::id>> m_removed;
    std::counting_semaphore<> m_sem;
    bool m_paused;
};

} //namespace details
} //namespace ilrd



#endif //ILRD_RD1645_THREADPOOL_HPP



