/********************************
 Name:     Alon Nov
 Exercise: async injection
 Date:	   26.03.2025
 Reviewer: Itai Hirschhorn

********************************/

#ifndef ILRD_RD1645_ASYNCINJECTION_HPP
#define ILRD_RD1645_ASYNCINJECTION_HPP

#include <chrono>               // milliseconds, system_clock::time_point, shared_ptr
#include <functional>           // std::function

#include "ISchedulerTask.hpp"   // SchedulerTask


namespace ilrd
{
namespace details   
{

class AsyncInjection
{
public:
    //if "func_" returns true: run again after "interval_", otherwise: deletes object
    AsyncInjection(const std::function<bool()>& func_, std::chrono::milliseconds interval_);

private: 
    class AsyncInjTask : public ISchedulerTask
    {
    public:
        AsyncInjTask(AsyncInjection& to_inject_);
        void Run();

    private:
        AsyncInjection& m_inject;
    };   

    ~AsyncInjection() = default;
    void PerformAction();
    std::function<bool()> m_func;
    std::chrono::milliseconds m_interval;
    std::shared_ptr<AsyncInjTask> m_task;
};

} //namespace details
} //namespace ilrd

#endif //ILRD_RD1645_ASYNCINJECTION_HPP



