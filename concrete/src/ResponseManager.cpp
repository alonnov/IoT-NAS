/********************************
 Name:     Alon Nov
 Exercise: concrete
 Date:	   06.04.2025
 Reviewer: 

********************************/

#include <iostream>
#include "Handleton.hpp"
#include "MinionManager.hpp"
#include "ResponseManager.hpp"    // Dispatcher, ACallback, Callback

namespace ilrd
{
namespace details   
{

ResponseManager::ResponseManager() : 
m_ticket_create_callback(*this, &ResponseManager::OnTicketCreate), 
m_ticket_done_callback(*this, &ResponseManager::OnTicketDone)
{
}

void ResponseManager::Init(std::shared_ptr<NBDProxy> nbd_)
{
    m_nbd = nbd_;

    Handleton<MinionManager>::GetInstance()->RegisterNewTicket(&m_ticket_create_callback);
}


void ResponseManager::OnTicketCreate(std::shared_ptr<Ticket> ticket_) 
{
    std::unique_lock<std::mutex>lock(m_mtx);
        
    m_tickets.emplace(ticket_->GetResult().m_uid, ticket_);
        
    ticket_->RegisterOnDone(&m_ticket_done_callback);
}


void ResponseManager::OnTicketDone(const TaskResult& result_) 
{
    m_nbd->OnTaskDone(result_);

    std::unique_lock<std::mutex>lock(m_mtx);
    
    m_tickets.erase(result_.m_uid);
}


} //namespace details
} //namespace ilrd




