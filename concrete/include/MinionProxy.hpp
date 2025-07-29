/********************************
 Name:     Alon Nov
 Exercise: concrete
 Date:	   06.04.2025
 Reviewer: 

********************************/

#ifndef ILRD_RD1645_MINIONPROXY_HPP
#define ILRD_RD1645_MINIONPROXY_HPP

#include "AMinionArgs.hpp"
#include "sockets.h"
#include "IMinionProxy.hpp"
#include "ResponseManager.hpp"

namespace ilrd
{
namespace details   
{

class MinionProxy : public IMinionProxy
{
public:
    MinionProxy(char* ip, char* port);
    ~MinionProxy();
    void SendWriteTask(const MinionArgs& args_, std::function<bool(const TaskResult&)> on_done_) override;
    void SendReadTask(const MinionArgs& args_, std::function<bool(const TaskResult&)> on_done_) override;
    int GetFD();
    void OnWakeup();

private:
    sockinfo* m_socket;
    std::mutex m_mtx;
    std::map<UID, std::function<bool(const TaskResult&)>> m_callbacks;
};




} //namespace details
} //namespace ilrd

#endif //ILRD_RD1645_MINIONPROXY_HPP



