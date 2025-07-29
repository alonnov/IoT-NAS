/********************************
 Name:     Alon Nov
 Exercise: concrete
 Date:	   06.04.2025
 Reviewer: 

********************************/
#include <cstring>
#include <iostream>

#include "Logger.hpp"
#include "UID.hpp"
#include "Handleton.hpp"
#include "ResponseManager.hpp"
#include "MinionManager.hpp"// API


namespace ilrd
{
namespace details   
{

Ticket::Ticket(std::list<std::shared_ptr<IMinionProxy>>& proxies_, const UID& uid_) :
m_proxies(proxies_), m_result(uid_)
{ 
    Handleton<Logger>::GetInstance()->Log("Ticket Created", Logger::DEBUG);
}


void Ticket::RegisterOnDone(ACallback<const TaskResult&>* callback_)
{
    m_dispatcher.Register(*callback_);
}


bool Ticket::OnMinionDone(const TaskResult& result_)
{
    Handleton<Logger>::GetInstance()->Log("Ticket: OnMinionDone called", Logger::DEBUG);
    
    if(m_result.m_status == RUNNING)
    {   
        m_result = result_;
        return false;
    }

    if(m_result.m_status == SUCCESS)
    {
        m_dispatcher.Notify(m_result);
        return true;
    }

    m_dispatcher.Notify(result_);

    return true;
}


const TaskResult& Ticket::GetResult()
{
    return m_result;
}

} //namespace details
} //namespace ilrd




