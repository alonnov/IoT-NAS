/********************************
 Name:     Alon Nov
 Exercise: concrete
 Date:	   06.04.2025
 Reviewer: 

********************************/

#ifndef ILRD_RD1645_NBDPROXY_HPP
#define ILRD_RD1645_NBDPROXY_HPP

#include <vector> 
#include <map>
#include <linux/nbd.h>      // nbd_reply

#include "Dispatcher.hpp"
#include "ATaskArgs.hpp"
#include "IInputProxy.hpp"  // Dispatcher

namespace ilrd
{
namespace details   
{

class NBDProxy : public IInputProxy
{
public:
    NBDProxy(int fd_);
    std::shared_ptr<ATaskArgs> GetTaskArgs(int fd_, MODE mode_);
    void Register(ACallback<const TaskResult&>* to_reg_);
    void OnTaskDone(const TaskResult& result_);
    
private:
    int m_fd;
    Dispatcher<const TaskResult&> m_dispatcher;
    std::mutex m_mtx;
    std::map<UID, nbd_request> m_requests;
    void ReadAll(char* buffer_, size_t count_);
    void WriteAll(char* buffer_, size_t count_);
};



} //namespace details
} //namespace ilrd

#endif //ILRD_RD1645_NBDPROXY_HPP



