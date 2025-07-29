/********************************
 Name:     Alon Nov
 Exercise: decoupling
 Date:	   18.03.2025
 Reviewer: Liad Tiblum

********************************/

#ifndef ILRD_RD1645_CALLBACK_HPP
#define ILRD_RD1645_CALLBACK_HPP

#include <unordered_set>// unordered_set
#include <algorithm>    // for_each
#include <queue>        // queue

#include "ACallback.hpp"

namespace ilrd
{
namespace details   
{

template <typename OBJ, typename ...EVENT>
class Callback : public ACallback<EVENT...>
{
public:
    Callback(OBJ& object_, void (OBJ::*fptr_notify_)(EVENT...), 
            void (OBJ::*fptr_notify_death_)() = nullptr);
    void Notify(EVENT... event);
    void NotifyDeath();

private:
    
    OBJ& m_object;
    void (OBJ::*m_notify)(EVENT...);
    void (OBJ::*m_notify_death)();
};


template <typename OBJ, typename ...EVENT>
Callback<OBJ, EVENT...>::Callback(OBJ& object_, void (OBJ::*fptr_notify_)(EVENT...), 
void (OBJ::*fptr_notify_death_)()) 
:
m_object(object_),
m_notify(fptr_notify_),
m_notify_death(fptr_notify_death_)
{
}


template <typename OBJ, typename ...EVENT>
void Callback<OBJ, EVENT...>::Notify(EVENT... event)
{
    (m_object.*m_notify)(event...);
}


template <typename OBJ, typename ...EVENT>
void Callback<OBJ, EVENT...>::NotifyDeath()
{
    if(m_notify_death != nullptr)
    {
        (m_object.*m_notify_death)();
    }
}


} //namespace details
} //namespace ilrd

#endif //ILRD_RD1645_CALLBACK_HPP



