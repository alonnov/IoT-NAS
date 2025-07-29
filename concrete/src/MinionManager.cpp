/********************************
 Name:     Alon Nov
 Exercise: concrete
 Date:	   06.04.2025
 Reviewer: 

********************************/

#include "AMinionArgs.hpp"
#include "iostream"
#include "Handleton.hpp"
#include "MinionProxy.hpp"
#include "MinionManager.hpp"// API

namespace ilrd
{
namespace details   
{

const size_t MINION_SIZE = 4194304;

MinionManager::MinionManager()
{
}

MinionCalc MinionManager::CalculateMinions(size_t offset_)
{
    MinionCalc calc;
    
    calc.m_minion = (offset_ / MINION_SIZE) % m_minions.size();
    calc.m_offset = offset_ % MINION_SIZE;
    calc.m_backup_minion = (calc.m_minion + 1) % m_minions.size();
    calc.m_backup_offset = calc.m_offset + MINION_SIZE;

    return calc;
}

void MinionManager::AddReadTask(std::shared_ptr<ITaskArgs> args_)
{
    auto read_args = std::static_pointer_cast<NBDReadArgs>(args_);

    MinionCalc calc = CalculateMinions(read_args->GetOffset());
    
    MinionArgs data_args(calc.m_offset, read_args->GetLength(), read_args->GetUID(), nullptr);
    MinionArgs backup_args(calc.m_backup_offset, read_args->GetLength(), read_args->GetUID(), nullptr);

    std::list<std::shared_ptr<IMinionProxy>> minions;
    minions.push_front(m_minions[calc.m_backup_minion]);
    minions.push_front(m_minions[calc.m_minion]);

    std::shared_ptr<Ticket> ticket = std::make_shared<Ticket>(minions, read_args->GetUID());
    m_dispatcher.Notify(ticket);  

    m_minions[calc.m_minion]->SendReadTask(data_args, [ticket](const TaskResult& res)
    {
        return ticket->OnMinionDone(res);
    });

    m_minions[calc.m_backup_minion]->SendReadTask(backup_args, [ticket](const TaskResult& res)
    {
        return ticket->OnMinionDone(res);
    });
}

void MinionManager::AddWriteTask(std::shared_ptr<ITaskArgs> args_)
{
    auto write_args = std::static_pointer_cast<NBDWriteArgs>(args_);
    
    MinionCalc calc = CalculateMinions(write_args->GetOffset());

    MinionArgs data_args(calc.m_offset, write_args->GetLength(), write_args->GetUID(), write_args->GetData());
    MinionArgs backup_args(calc.m_backup_offset, write_args->GetLength(), write_args->GetUID(), write_args->GetData());

    std::list<std::shared_ptr<IMinionProxy>> minions;
    minions.push_front(m_minions[calc.m_backup_minion]);
    minions.push_front(m_minions[calc.m_minion]);

    std::shared_ptr<Ticket> ticket = std::make_shared<Ticket>(minions, write_args->GetUID());
    m_dispatcher.Notify(ticket);    

    m_minions[calc.m_minion]->SendWriteTask(data_args, [ticket](const TaskResult& res)
    {
        return ticket->OnMinionDone(res);
    });

    m_minions[calc.m_backup_minion]->SendWriteTask(backup_args, [ticket](const TaskResult& res)
    {
        return ticket->OnMinionDone(res);
    });
}


void MinionManager::RegisterNewTicket(ACallback<std::shared_ptr<Ticket>>* ticket_)
{
    m_dispatcher.Register(*ticket_);
}


void MinionManager::Init(std::vector<std::shared_ptr<IMinionProxy>>& minions_)
{
    for(auto i : minions_)
    {
        Handleton<Reactor>::GetInstance()->Register(i->GetFD(), READ, [i](int fd, MODE mode)
        {
            (void)fd;
            (void)mode;
            i->OnWakeup();
        });
        
        m_minions.push_back(i);
    }
}


} //namespace details
} //namespace ilrd




