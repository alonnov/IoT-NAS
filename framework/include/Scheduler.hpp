/********************************
 Name:     Alon Nov
 Exercise: scheduler
 Date:	   25.03.2025
 Reviewer: Sahar Moalem

********************************/

#ifndef ILRD_RD1645_SCHEDULER_HPP
#define ILRD_RD1645_SCHEDULER_HPP

#include <memory>
#include <queue>                // std::priority_queue
#include <mutex>                // std::mutex
#include <chrono>               // milliseconds, system_clock::time_point, shared_ptr
#include <atomic>               // std::atomic_uint, std::atomic_bool
#include <csignal>              // sigval_t

#include "ISchedulerTask.hpp"   // SchedulerTask


namespace ilrd
{
namespace details   
{


class Scheduler
{
public:
    Scheduler();
    ~Scheduler();

    // add a task for the scheduler to execute
    void AddTask(std::shared_ptr<ISchedulerTask> task_, std::chrono::milliseconds until_exe_);

private: 
    timer_t m_timer;
    itimerspec m_timer_spec;
    std::atomic_uint m_counter;
    std::atomic_bool m_running;
    std::mutex m_queue_mtx;
    std::mutex m_timer_mtx;

    struct Compare {
        bool operator()(
            const std::pair<std::chrono::system_clock::time_point, ISchedulerTask*>& task1,
            const std::pair<std::chrono::system_clock::time_point, ISchedulerTask*>& task2) const;
    };

    std::priority_queue<std::pair<
    std::chrono::system_clock::time_point, ISchedulerTask*>,
    std::vector<std::pair<std::chrono::system_clock::time_point, ISchedulerTask*>>,
      Compare> m_queue;
    void ResetTimer();
    void ThreadHelper();
    static void ThreadFunc(sigval_t);
};



} //namespace details
} //namespace ilrd

#endif //ILRD_RD1645_SCHEDULER_HPP



