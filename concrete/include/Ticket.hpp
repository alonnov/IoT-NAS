/********************************
 Name:     Alon Nov
 Exercise: concrete
 Date:	   06.04.2025
 Reviewer: 

********************************/

#ifndef ILRD_RD1645_TICKET_HPP
#define ILRD_RD1645_TICKET_HPP

#include <list> 

#include "ATaskArgs.hpp"
#include "IMinionProxy.hpp" // IMinionProxy
#include "Dispatcher.hpp"    // ACallback, Dispatcher
#include "utils.hpp"

namespace ilrd
{
namespace details   
{

class Ticket
{
public:
    Ticket(std::list<std::shared_ptr<IMinionProxy>>& proxies_, const UID& uid_);
    void RegisterOnDone(ACallback<const TaskResult&>* callback_);
    // void Unregister();
    const TaskResult& GetResult();
    bool OnMinionDone(const TaskResult& result_);

private:
    Dispatcher<const TaskResult&> m_dispatcher;
    std::list<std::shared_ptr<IMinionProxy>>& m_proxies;
    TaskResult m_result;
};


} //namespace details
} //namespace ilrd

#endif //ILRD_RD1645_TICKET_HPP



