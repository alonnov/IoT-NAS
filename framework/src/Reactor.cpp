/********************************
 Name:     Alon Nov
 Exercise: FD event handling
 Date:	   24.03.2025
 Reviewer: 

********************************/

#include <vector>       // std::vector

#include "Reactor.hpp"   // API

namespace ilrd
{
namespace details
{

Reactor::Reactor() : m_running(false)
{
}

void Reactor::Init(IListener* listener_)
{
    m_listener = std::unique_ptr<IListener>(listener_);
}


void Reactor::Register(int fd_, MODE mode_, std::function<void(int, MODE)> callback_)
{
    m_fds[std::make_pair(fd_, mode_)] = callback_;
}


void Reactor::Unregister(int fd_, MODE mode_)
{
    m_fds.erase({fd_, mode_});
}


void Reactor::Run()
{
    std::vector<std::pair<int, MODE>> listeners;
    m_running = true;

    while(m_running)
    {
        listeners.clear();

        for(auto& i : m_fds)
        {
            listeners.emplace_back(i.first);
        }

        std::vector<std::pair<int, MODE>> ready = m_listener->Listen(listeners);

        for(auto& i : ready)
        {
            m_fds.at(std::make_pair(i.first, i.second))(i.first, i.second);
        }
    }
}


void Reactor::Stop()
{
    m_running = false;
}


} //namespace details
} //namespace ilrd




