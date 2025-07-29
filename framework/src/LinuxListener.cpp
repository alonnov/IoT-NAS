/********************************
 Name:     Alon Nov
 Exercise: FD event handling
 Date:	   24.03.2025
 Reviewer: Liad Tiblum

********************************/

#include <poll.h>       // pollfd
#include <stdexcept>

#include "LinuxListener.hpp"

namespace ilrd
{
namespace details
{

std::vector<std::pair<int, MODE>> LinuxListener::Listen(std::vector<std::pair<int, MODE>>& fds_)
{
    nfds_t size = fds_.size();

    for(auto& i : fds_)
    {
        if(i.second == READ)
        {
            m_poll_fds.emplace_back(i.first, POLLIN, 0);
        }
        else
        {
            m_poll_fds.emplace_back(i.first, POLLOUT, 0);
        }
    }

    int pnum = poll(m_poll_fds.data(), size, -1);
    if(pnum == -1)
    {
        throw std::runtime_error("poll error");
    }

    std::vector<std::pair<int, MODE>> result;

    for(size_t i = 0; i < size; ++i)
    { 
        if(m_poll_fds[i].revents & POLLIN)
        {
            result.emplace_back(m_poll_fds[i].fd, READ);
        }     

        else if(m_poll_fds[i].revents & POLLOUT)
        {
            result.emplace_back(m_poll_fds[i].fd, WRITE);
        }                                
    }

    m_poll_fds.clear();

    return result;
}

} //namespace details
} //namespace ilrd



