/********************************
 Name:     Alon Nov
 Exercise: async injection
 Date:	   26.03.2025
 Reviewer: Itai Hirschhorn

********************************/

#include "Scheduler.hpp"
#include "Handleton.hpp"
#include "AsyncInjection.hpp"   // API

namespace ilrd
{
namespace details
{


/////////////////////////////AsyncInjection/////////////////////////////////////
AsyncInjection::AsyncInjection(const std::function<bool()>& func_, 
    std::chrono::milliseconds interval_) :
m_func(std::bind(func_)), m_interval(interval_), m_task(std::make_shared<AsyncInjTask>(*this))
{
    Handleton<Scheduler>::GetInstance()->AddTask(m_task, m_interval);
}

void AsyncInjection::PerformAction()
{
    if(true == m_func())
    {
        Handleton<Scheduler>::GetInstance()->AddTask(m_task, m_interval);
    }
    else
    {
        delete this;
    }
}


/////////////////////////////AsyncInjTask/////////////////////////////////////
AsyncInjection::AsyncInjTask::AsyncInjTask(AsyncInjection& to_inject_) : 
m_inject(to_inject_)
{
}

void AsyncInjection::AsyncInjTask::Run()
{
    m_inject.PerformAction();
}


} //namespace details
} //namespace ilrd




