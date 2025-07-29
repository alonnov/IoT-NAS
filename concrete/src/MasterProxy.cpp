/********************************
 Name:     Alon Nov
 Exercise: concrete
 Date:	   06.04.2025
 Reviewer: 

********************************/

#include "Logger.hpp"
#include "AMinionArgs.hpp"
#include "Handleton.hpp"
#include "Reactor.hpp"
#include "Factory.hpp"
#include "MasterProxy.hpp"    // API

namespace ilrd
{
namespace details   
{
MasterProxy::MasterProxy(char* ip_, char* port_)
{
    m_socket = UDPCreate(ip_, port_);
    SockBind(m_socket);
    
    Handleton<Reactor>::GetInstance()->Register(SockGetFD(m_socket), READ, [this]
        (int fd, MODE mode)
        {
            (void)fd;
            (void)mode;
            GetTaskArgs(fd, READ);
        });

    Handleton<Factory<uint32_t, ATaskArgs, std::shared_ptr<ANBDArgs>>>::GetInstance()->Register(READ, [this](std::shared_ptr<ANBDArgs> nbd_args)
    {
        return std::make_shared<MinionReadArgs>(nbd_args->GetUID(), this, nbd_args->GetOffset(), nbd_args->GetLength());
    });

    Handleton<Factory<uint32_t, ATaskArgs, std::shared_ptr<ANBDArgs>>>::GetInstance()->Register(WRITE, [this](std::shared_ptr<ANBDArgs> nbd_args)
    {
        std::shared_ptr<MinionWriteArgs> minion_args = std::make_shared<MinionWriteArgs>(nbd_args->GetUID(), this, nbd_args->GetOffset(), nbd_args->GetLength(), std::static_pointer_cast<NBDWriteArgs>(nbd_args)->GetData());

        return minion_args;
    });
}


MasterProxy::~MasterProxy()
{
    SockDestroy(m_socket);
    // delete m_send_addr;
}


std::shared_ptr<ATaskArgs> MasterProxy::GetTaskArgs(int fd_, MODE mode_)
{
    (void)fd_;
    (void)mode_;

    char buffer[BUFSIZ] = {0};
    if(0 > UDPRecvFrom(SockGetFD(m_socket), &m_send_addr, buffer, BUFSIZ))
    {
        Handleton<Logger>::GetInstance()->Log("MasterProxy: UDP receive error", Logger::ERROR);
    }

    Handleton<Logger>::GetInstance()->Log("MasterProxy: UDP msg received", Logger::DEBUG);

    MESSAGE_TYPE class_type = *reinterpret_cast<MESSAGE_TYPE*>(buffer + sizeof(uint32_t));

    std::shared_ptr<AMessageRequest> request;
    std::shared_ptr<ATaskArgs> args;

    if(class_type == READ_REQUEST)
    {
        request = std::make_shared<MessageReadRequest>();

        request->FromBuffer(buffer);
        
        args = Handleton<Factory<uint32_t, ATaskArgs, std::shared_ptr<ANBDArgs>>>::GetInstance()->Create(class_type, 
            std::make_shared<NBDReadArgs>(request->GetUID(), request->GetOffset(), request->GetLength()));
    }
    
    else if(class_type == WRITE_REQUEST)
    {
        request = std::make_shared<MessageWriteRequest>();

        request->FromBuffer(buffer);

        args = Handleton<Factory<uint32_t, ATaskArgs, std::shared_ptr<ANBDArgs>>>::GetInstance()->Create(class_type, 
            std::make_shared<NBDWriteArgs>(request->GetUID(), request->GetOffset(), request->GetLength(), std::static_pointer_cast<MessageWriteRequest>(request)->GetData()));   
    }

    return args; 
}

int MasterProxy::GetFD()
{
    return SockGetFD(m_socket);
}

void MasterProxy::SendReadResponse(const UID& uid_, STATUS status_, size_t length_, std::shared_ptr<char[]> read_)
{   
    MessageReadResponse msg(uid_, status_, length_, read_);
    size_t size = msg.GetSize();

    char* buffer = new char[size];
    msg.ToBuffer(buffer);

    if(0 > UDPSendTo(SockGetFD(m_socket), &m_send_addr, buffer, size))
    {
        Handleton<Logger>::GetInstance()->Log("MasterProxy::SendReadResponse: UDPSendTo error", Logger::ERROR);
    }
    Handleton<Logger>::GetInstance()->Log("MasterProxy: read response sent", Logger::DEBUG);

    delete[] buffer;
}


void MasterProxy::SendWriteResponse(const UID& uid_, STATUS status_)
{
    MessageWriteResponse msg(uid_, status_);
    size_t size = msg.GetSize();

    char* buffer = new char[size];
    msg.ToBuffer(buffer);

    if(0 > UDPSendTo(SockGetFD(m_socket), &m_send_addr, buffer, size))
    {
        Handleton<Logger>::GetInstance()->Log("MasterProxy::SendWriteResponse: UDP send error", Logger::ERROR);
    }
    Handleton<Logger>::GetInstance()->Log("MasterProxy: write response sent", Logger::DEBUG);

    delete[] buffer;
}


} //namespace details
} //namespace ilrd




