/********************************
 Name:     Alon Nov
 Exercise: DLL loader / Dir monitor
 Date:	   19.03.2025
 Reviewer: Gil Freedman

********************************/

#include <sys/inotify.h>    // inotify_init1, inotify_add_watch, inotify_rm_watch

#include "DirMonitor.hpp"   // API, jthread, std::string

namespace ilrd
{
namespace details
{

DirMonitor::DirMonitor(std::string path_name_) : m_running(false), m_path_name(path_name_)
{
    m_fd = inotify_init();
    if(m_fd == -1)
    {
       throw std::runtime_error("inotify_init error");
    }

    m_wd = inotify_add_watch(m_fd, path_name_.c_str(), IN_CREATE | IN_DELETE);
    if(m_wd == -1)
    {
        close(m_fd);
        throw std::runtime_error("inotify_add_watch error");
    }
}

DirMonitor::~DirMonitor()
{
    m_running = false;

    inotify_rm_watch(m_fd, m_wd);
    close(m_fd);
}


void DirMonitor::RegisterCreate(ACallback<const std::string&>& callback)
{
    m_create_dispatcher.Register(callback);
}


void DirMonitor::RegisterDelete(ACallback<const std::string&>& callback)
{
    m_delete_dispatcher.Register(callback);
}


void DirMonitor::UnregisterCreate(ACallback<const std::string&>& callback)
{
    m_create_dispatcher.Unregister(callback);
}


void DirMonitor::UnregisterDelete(ACallback<const std::string&>& callback)
{
    m_delete_dispatcher.Unregister(callback);
}


void DirMonitor::Run()
{
    m_thread = std::jthread([this](){ ThreadFunc();});
}


void DirMonitor::ThreadFunc()
{
    m_running = true;
    
    while(m_running == true)
    {     
        char buffer[PATH_MAX] = {0};
        ssize_t len = read(m_fd, buffer, sizeof(buffer));
        if(len == -1 && errno != EAGAIN)
        {
            throw std::runtime_error("inotify read error");
        }

        inotify_event *event = nullptr;
        for(size_t i = 0; i < static_cast<size_t>(len); 
            i += sizeof(inotify_event) + event->len)
        {
            event = reinterpret_cast<inotify_event*>(&buffer[i]);

            if(event->mask & IN_CREATE)
            {
                m_create_dispatcher.Notify(m_path_name + "/" + event->name);
            }

            else if(event->mask & IN_DELETE)
            {
                m_delete_dispatcher.Notify(m_path_name + "/" + event->name);
            }           
        }          
    }
}


} //namespace details
} //namespace ilrd




