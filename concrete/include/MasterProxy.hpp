/********************************
 Name:     Alon Nov
 Exercise: concrete
 Date:	   06.04.2025
 Reviewer: 

********************************/

#ifndef ILRD_RD1645_MASTERPROXY_HPP
#define ILRD_RD1645_MASTERPROXY_HPP

#include <map>

#include "udp.h"
#include "Message.hpp"
#include "ATaskArgs.hpp"
#include "IInputProxy.hpp"

namespace ilrd
{
namespace details   
{

class MasterProxy final : public IInputProxy
{
public:
    MasterProxy(char* ip_, char* port_);
    ~MasterProxy();
    std::shared_ptr<ATaskArgs> GetTaskArgs(int fd_, MODE mode_);
    int GetFD();
    void SendReadResponse(const UID& uid_, STATUS status_, size_t length_, std::shared_ptr<char[]> read_);
    void SendWriteResponse(const UID& uid_, STATUS status_);

private:
    sockinfo* m_socket;
    sockaddr m_send_addr;
};



} //namespace details
} //namespace ilrd

#endif //ILRD_RD1645_MASTERPROXY_HPP



