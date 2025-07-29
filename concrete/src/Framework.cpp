/********************************
 Name:     Alon Nov
 Exercise: framework
 Date:	   27.03.2025
 Reviewer: 

********************************/

#include "Handleton.hpp"    // Handleton
#include "ThreadPool.hpp"   // Threadpool, AddTask
#include "IListener.hpp"    // MODE
#include "LinuxListener.hpp"// LinuxListener
#include "Logger.hpp"       // Logger, Log
#include "Framework.hpp"    // API, ICommand, ITaskArgs

using namespace ilrd::details;

///////////////////////////Framework///////////////////////////////
Framework::Framework(
const std::list<FrameworkFDs>& callbacks_, 
    const std::list<std::pair<int, std::function<std::shared_ptr<ICommand>()>>>& factory_funcs_, 
        const std::string& plugins_dir_) : 
m_monitor(plugins_dir_), m_callback(m_loader, &DllLoader::Load)
{   
    try
    {
        m_monitor.RegisterCreate(m_callback); 

        Handleton<Reactor>::GetInstance()->Init(new LinuxListener());
    }
    catch(...)
    {
        Handleton<Logger>::GetInstance()->Log("Framework: constructor bad alloc", Logger::ERROR);
        throw;
    }
    
    for(auto& i : callbacks_)
    {
        Handleton<Reactor>::GetInstance()->Register(i.m_fd, i.m_mode, 
        [i](int fd, MODE mode)
        {
            try
            {
                std::shared_ptr<ITaskArgs> args_ptr = i.m_proxy->GetTaskArgs(fd, mode);
                if(!args_ptr)
                {
                    return;
                }
            
                std::shared_ptr<ITPTask> task_ptr = std::make_shared<FWTPTask>(args_ptr);           

                Handleton<ThreadPool>::GetInstance()->AddTask(task_ptr);
            }
            catch(...)
            {
                Handleton<Logger>::GetInstance()->Log("Framework: constructor bad alloc", Logger::ERROR);
                throw;
            }
        });
    }

    for(auto& i : factory_funcs_)
    {
        Handleton<Factory<int, ICommand>>::GetInstance()->Register(i.first, i.second);
    }
}


void Framework::Run()
{
    m_monitor.Run();
    Handleton<Reactor>::GetInstance()->Run();
}



///////////////////////////FWTPTask//////////////////////////////////////
Framework::FWTPTask::FWTPTask(std::shared_ptr<ITaskArgs> task_args_) : 
m_task_args(task_args_)
{
}


void Framework::FWTPTask::Run()
{
    // try
    // {
        auto cmd = Handleton<Factory<int, ICommand>>::GetInstance()->
            Create(m_task_args->GetKey());
        
        std::pair<std::function<bool()>, std::chrono::milliseconds> async_injection = 
            cmd->Run(m_task_args);

        if(async_injection.first)
        {  
            new AsyncInjection(async_injection.first, async_injection.second);   
        }
    // }
    // catch(...)
    // {
    //     //Handleton<Logger>::GetInstance()->Log("Framework: task creation error", Logger::ERROR);
    //     throw;
    // }
}




