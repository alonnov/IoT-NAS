/********************************
 Name:     Alon Nov
 Exercise: concrete
 Date:	   06.04.2025
 Reviewer: 

********************************/

#ifndef ILRD_RD1645_RESPONSEMANAGER_HPP
#define ILRD_RD1645_RESPONSEMANAGER_HPP

#include <vector> 
#include <map>

#include "ATaskArgs.hpp"
#include "NBDProxy.hpp"
#include "Ticket.hpp"
#include "UID.hpp"         // UID
#include "Dispatcher.hpp"    // Dispatcher, ACallback, Callback

namespace ilrd
{
namespace details   
{

class ResponseManager
{
public:
    ResponseManager();
    void Init(std::shared_ptr<NBDProxy> nbd_);

private:
    void OnTicketCreate(std::shared_ptr<Ticket> ticket_);
    void OnTicketDone(const TaskResult& result_);
    Callback<ResponseManager, std::shared_ptr<Ticket>> m_ticket_create_callback;
    Callback<ResponseManager, const TaskResult&> m_ticket_done_callback;
    std::mutex m_mtx;
    std::map<UID, std::shared_ptr<Ticket>> m_tickets;
    std::shared_ptr<NBDProxy> m_nbd;
};



} //namespace details
} //namespace ilrd

#endif //ILRD_RD1645_RESPONSEMANAGER_HPP



