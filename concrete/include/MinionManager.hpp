/********************************
 Name:     Alon Nov
 Exercise: concrete
 Date:	   06.04.2025
 Reviewer: 

********************************/

#ifndef ILRD_RD1645_MINIONMANAGER_HPP
#define ILRD_RD1645_MINIONMANAGER_HPP

#include <vector> 
#include <map>

#include "Ticket.hpp"
#include "Reactor.hpp"      // Reactor
#include "IMinionProxy.hpp"
#include "Dispatcher.hpp"    // Dispatcher

namespace ilrd
{
namespace details   
{

struct MinionCalc
{
    size_t m_minion;
    size_t m_offset;
    size_t m_backup_minion;
    size_t m_backup_offset;
};

class MinionManager
{
public:
    MinionManager();
    void Init(std::vector<std::shared_ptr<IMinionProxy>>& minions_);
    void AddReadTask(std::shared_ptr<ITaskArgs> args_);
    void AddWriteTask(std::shared_ptr<ITaskArgs> args_);
    void RegisterNewTicket(ACallback<std::shared_ptr<Ticket>>* ticket_);
    
private:
    std::vector<std::shared_ptr<IMinionProxy>> m_minions;
    Dispatcher<std::shared_ptr<Ticket>> m_dispatcher;
    MinionCalc CalculateMinions(size_t offset_);
};



} //namespace details
} //namespace ilrd

#endif //ILRD_RD1645_MINIONMANAGER_HPP



