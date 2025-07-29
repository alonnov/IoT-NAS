/********************************
 Name:     Alon Nov
 Exercise: DLL loader / Dir monitor
 Date:	   19.03.2025
 Reviewer: Gil Freedman

********************************/

#ifndef ILRD_RD1645_DIRMONITOR_HPP
#define ILRD_RD1645_DIRMONITOR_HPP

#include <thread>           //jthread
#include <string>           //std::string

#include "Dispatcher.hpp"   // Dispatcher

namespace ilrd
{
namespace details
{

class DirMonitor
{
public:
    DirMonitor(std::string path_name = "./");
    ~DirMonitor();
    void RegisterCreate(ACallback<const std::string&>& callback);
    void RegisterDelete(ACallback<const std::string&>& callback);
    void UnregisterCreate(ACallback<const std::string&>& callback);
    void UnregisterDelete(ACallback<const std::string&>& callback);
    void Run();

private: 
    std::jthread m_thread;
    int m_wd;
    int m_fd;
    Dispatcher<const std::string&> m_create_dispatcher;
    Dispatcher<const std::string&> m_delete_dispatcher;
    bool m_running;
    void ThreadFunc();
    std::string m_path_name;
};


} //namespace details
} //namespace ilrd

#endif //ILRD_RD1645_DIRMONITOR_HPP



