/********************************
 Name:     Alon Nov
 Exercise: FD event handling
 Date:	   24.03.2025
 Reviewer: Liad Tiblum

********************************/

#ifndef ILRD_RD1645_REACTOR_HPP
#define ILRD_RD1645_REACTOR_HPP

#include <memory>       // unique_ptr
#include <functional>   // std::function
#include <map>          // std::map

#include "LinuxListener.hpp"

namespace ilrd
{
namespace details
{

class Reactor
{

public:
    explicit Reactor();
    void Init(IListener* listener_);
    void Register(int fd_, MODE mode_, std::function<void(int, MODE)> callback_);
    void Unregister(int fd_, MODE mode_);
    void Run();
    void Stop();

private: 
    std::unique_ptr<IListener> m_listener;
    std::map<std::pair<int, MODE>, std::function<void(int, MODE)>> m_fds;
    bool m_running;
};


} //namespace details
} //namespace ilrd

#endif //ILRD_RD1645_REACTOR_HPP



