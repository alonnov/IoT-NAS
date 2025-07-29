/********************************
 Name:     Alon Nov
 Exercise: concrete
 Date:	   06.04.2025
 Reviewer: 

********************************/

#include "Logger.hpp"
#include "udp.h"
#include "Factory.hpp"
#include "Handleton.hpp"
#include "ResponseManager.hpp"
#include "Message.hpp"
#include "MinionProxy.hpp"
#include <iostream>

namespace ilrd
{
namespace details   
{


MinionProxy::MinionProxy(char* ip, char* port)
{
    m_socket = UDPCreate(ip, port);
}


MinionProxy::~MinionProxy()
{
    SockDestroy(m_socket);
}


void MinionProxy::SendReadTask(const MinionArgs& args_, std::function<bool(const TaskResult&)> on_done_)
{
    auto msg = std::make_shared<MessageReadRequest>(args_.m_offset, args_.m_length, args_.m_uid); 

    std::unique_lock<std::mutex>lock(m_mtx);

    // m_callbacks[args_.m_uid] = on_done_;
    m_callbacks.emplace(args_.m_uid, on_done_);
    // if (!temp.second)
    // {
    //     std::cout << "Is equal idiot read: " << (temp.first->first == args_.m_uid) << std::endl;
    //     throw std::runtime_error("idiot read");
    // }
    
    size_t size = msg->GetSize();
    char* buffer = new char[size];
    msg->ToBuffer(buffer);

    if(0 > UDPSendTo(SockGetFD(m_socket), SockGetAddr(m_socket), buffer, size))
    {
        Handleton<Logger>::GetInstance()->Log("MinionProxy::SendReadTask: UDPSendTo error", Logger::ERROR);
    }
    Handleton<Logger>::GetInstance()->Log("MinionProxy: read request sent", Logger::DEBUG);

    delete[] buffer;
}


void MinionProxy::SendWriteTask(const MinionArgs& args_, std::function<bool(const TaskResult&)> on_done_)
{
    auto msg = std::make_shared<MessageWriteRequest>(args_.m_offset, args_.m_length, args_.m_uid, args_.m_data);  

    std::unique_lock<std::mutex>lock(m_mtx);

    m_callbacks.emplace(args_.m_uid, on_done_);

    size_t size = msg->GetSize();
    char* buffer = new char[size];
    msg->ToBuffer(buffer);

    if(0 > UDPSendTo(SockGetFD(m_socket), SockGetAddr(m_socket),buffer, size))
    {
        Handleton<Logger>::GetInstance()->Log("MinionProxy: UDP send write error", Logger::ERROR);
    }
    Handleton<Logger>::GetInstance()->Log("MinionProxy: write request sent", Logger::DEBUG);

    delete[] buffer;
}

int MinionProxy::GetFD()
{
    return SockGetFD(m_socket);
}

void MinionProxy::OnWakeup()
{
    char buffer[BUFSIZ] = {0};
    
    std::unique_lock<std::mutex>lock(m_mtx);

    if(0 > UDPRecv(GetFD(), buffer, BUFSIZ))
    {
        Handleton<Logger>::GetInstance()->Log("MinionProxy: UDP receive error", Logger::ERROR);
    }

    Handleton<Logger>::GetInstance()->Log("MinionProxy: UDP msg received", Logger::DEBUG);

    MESSAGE_TYPE class_type = *reinterpret_cast<MESSAGE_TYPE*>(buffer + sizeof(uint32_t));
    
    std::shared_ptr<AMessageResponse> response;

    if(class_type == READ_RESPONSE)
    {
        response = std::make_shared<MessageReadResponse>();
    }
    
    else if(class_type == WRITE_RESPONSE)
    {
        response = std::make_shared<MessageWriteResponse>();
    }

    response->FromBuffer(buffer);

    TaskResult res(response->GetUID(), response->GetStatus(), response->GetLength(), response->GetData());

    if(m_callbacks.at(res.m_uid)(res))
    {
        m_callbacks.erase(res.m_uid);
    }
}


MinionArgs::MinionArgs(size_t offset_, size_t length_, const UID& uid_, std::shared_ptr<char[]> data_) :
m_offset(offset_), m_length(length_), m_uid(uid_), m_data(data_)
{
}


} //namespace details
} //namespace ilrd




