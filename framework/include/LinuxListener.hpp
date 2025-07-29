/********************************
 Name:     Alon Nov
 Exercise: FD event handling
 Date:	   24.03.2025
 Reviewer: Liad Tiblum

********************************/

#ifndef ILRD_RD1645_LINUXLISTENER_HPP
#define ILRD_RD1645_LINUXLISTENER_HPP

#include <poll.h>       // pollfd

#include "IListener.hpp"

namespace ilrd
{
namespace details
{

class LinuxListener : public IListener
{
public:
    std::vector<std::pair<int, MODE>> Listen(std::vector<std::pair<int, MODE>>& fds_);
private:
    std::vector<pollfd> m_poll_fds;
};


} //namespace details
} //namespace ilrd

#endif //ILRD_RD1645_LINUXLISTENER_HPP



