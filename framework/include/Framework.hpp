/********************************
 Name:     Alon Nov
 Exercise: framework
 Date:	   27.03.2025
 Reviewer: 

********************************/

#ifndef ILRD_RD1645_FRAMEWORK_HPP
#define ILRD_RD1645_FRAMEWORK_HPP

#include <list>                 // std::list

#include "DllLoader.hpp"
#include "Reactor.hpp"
#include "DirMonitor.hpp"
#include "Factory.hpp"
#include "AsyncInjection.hpp"
#include "ITPTask.hpp"
#include "IListener.hpp"
#include "IInputProxy.hpp"
#include "ITaskArgs.hpp"
#include "ICommand.hpp"

namespace ilrd
{
namespace details   
{

class Framework
{
public:
    struct FrameworkFDs
    {
        int m_fd;
        MODE m_mode;
        std::shared_ptr<IInputProxy> m_proxy;
    };

    Framework(const std::list<FrameworkFDs>& callbacks_, 
        const std::list<std::pair<int, std::function<std::shared_ptr<ICommand>()>>>& factory_funcs_, 
            const std::string& plugins_dir_);
    void Run();

private:    
    DirMonitor m_monitor;
    DllLoader m_loader;
    Callback<DllLoader, const std::string&> m_callback;
    
    class FWTPTask : public ITPTask
    {
    public:
        FWTPTask(std::shared_ptr<ITaskArgs> task_args_);
        void Run();
    private:
        std::shared_ptr<ITaskArgs> m_task_args;
    };
};



} //namespace details
} //namespace ilrd

#endif //ILRD_RD1645_FRAMEWORK_HPP



