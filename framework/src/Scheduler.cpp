/********************************
 Name:     Alon Nov
 Exercise: scheduler
 Date:	   25.03.2025
 Reviewer: Sahar Moalem

********************************/

#include <chrono>               // milliseconds, system_clock::time_point, shared_ptr

#include "Scheduler.hpp" 

namespace ilrd
{
namespace details   
{

Scheduler::Scheduler() : m_timer_spec()
{
    sigevent event;
    event.sigev_notify = SIGEV_THREAD;
    event.sigev_value.sival_ptr = this;
    event.sigev_notify_function = &ThreadFunc;
    event.sigev_notify_attributes = 0;

    if(0 != timer_create(CLOCK_REALTIME, &event, &m_timer))
    {
        throw "timer creation error";
    }
}


Scheduler::~Scheduler()
{
    m_running.wait(true);
}


void Scheduler::AddTask(std::shared_ptr<ISchedulerTask> task_, std::chrono::milliseconds until_exe_)
{
    {
    std::unique_lock lock(m_queue_mtx);

        m_queue.push(std::make_pair(
         std::chrono::system_clock::now() + until_exe_, task_.get()));
    }

    ++m_counter;

    ResetTimer();
}


void Scheduler::ThreadFunc(sigval_t val_)
{
    reinterpret_cast<Scheduler*>(val_.sival_ptr)->ThreadHelper();
}


void Scheduler::ThreadHelper()
{
    ISchedulerTask* temp = NULL;

    m_running = true;

    {
    std::unique_lock lock(m_queue_mtx);

        temp = m_queue.top().second;   
        m_queue.pop(); 
    }

    ResetTimer();
  
    temp->Run();

    --m_counter;
    if(m_counter.load() == 0)
    {
        m_running = false;
        m_running.notify_one();
    }
}



void Scheduler::ResetTimer()
{      
    std::chrono::system_clock::time_point temp;

    {  
    std::unique_lock lock(m_queue_mtx);
        if(m_queue.empty())
        {
            return;
        }
        temp = m_queue.top().first;
    }

    m_timer_spec.it_value = 
        {std::chrono::system_clock::to_time_t(temp), 
        (temp - std::chrono::floor<std::chrono::seconds>(temp)).count()};

    std::unique_lock lock(m_timer_mtx);
        if(0 != timer_settime(m_timer, TIMER_ABSTIME, &m_timer_spec, NULL))
        {
            throw "timer set error";
        }
}

bool Scheduler::Compare::operator()(
    const std::pair<std::chrono::system_clock::time_point, ISchedulerTask*>& task1,
    const std::pair<std::chrono::system_clock::time_point, ISchedulerTask*>& task2) const
{
    return task1.first > task2.first;
}


} //namespace details
} //namespace ilrd




