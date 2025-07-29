/********************************
 Name:     Alon Nov
 Exercise: decoupling
 Date:	   18.03.2025
 Reviewer: Liad Tiblum

********************************/

#ifndef ILRD_RD1645_ACALLBACK_HPP
#define ILRD_RD1645_ACALLBACK_HPP

#include <unordered_set>// unordered_set
#include <algorithm>    // for_each
#include <queue>        // queue

#include "Dispatcher.hpp"

namespace ilrd
{
namespace details   
{

template <typename ...EVENT>
class ACallback
{
public:
    virtual ~ACallback();
    virtual void Notify(EVENT... event) = 0;
    virtual void NotifyDeath() = 0;

    void SetDispatcher(Dispatcher<EVENT...>* dispatcher);

private:
    Dispatcher<EVENT...>* m_dispatcher;
};


template <typename ...EVENT>
ACallback<EVENT...>::~ACallback()
{
    if(m_dispatcher != nullptr)
    {
        m_dispatcher->Unregister(*this);
    }
}

template <typename ...EVENT>
void ACallback<EVENT...>::SetDispatcher(Dispatcher<EVENT...>* dispatcher)
{
    m_dispatcher = dispatcher;
}




} //namespace details
} //namespace ilrd

#endif //ILRD_RD1645_ACALLBACK_HPP



