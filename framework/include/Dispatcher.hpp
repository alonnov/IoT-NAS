/********************************
 Name:     Alon Nov
 Exercise: decoupling
 Date:	   18.03.2025
 Reviewer: Liad Tiblum

********************************/

#ifndef ILRD_RD1645_DISPATCHER_HPP
#define ILRD_RD1645_DISPATCHER_HPP

#include <unordered_set>// unordered_set
#include <algorithm>    // for_each
#include <queue>        // queue

namespace ilrd
{
namespace details   
{

template <typename ...EVENT>
class ACallback;

template <typename ...EVENT>
class Dispatcher
{
public:
    ~Dispatcher();
    void Register(ACallback<EVENT...>& callback);
    void Unregister(ACallback<EVENT...>& callback);
    void Notify(EVENT... event);

private:
    void UpdateSubs();
    std::unordered_set<ACallback<EVENT...>*> m_subs;
    std::queue<std::pair<bool, ACallback<EVENT...>*>> m_queue;
};


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

/////////////////////////////Dispatcher//////////////////////////////////////
template <typename ...EVENT>
Dispatcher<EVENT...>::~Dispatcher()
{
    UpdateSubs();

        std::for_each(m_subs.begin(), m_subs.end(), 
                [](ACallback<EVENT...>* iter)
                { 
                    try
                    {
                        iter->NotifyDeath();
                    }
                    catch(...)
                    {
                    }
                    iter->SetDispatcher(nullptr);
                });
}


template <typename ...EVENT>
void Dispatcher<EVENT...>::Register(ACallback<EVENT...>& callback)
{
    m_queue.push({true, &callback});
    callback.SetDispatcher(this);
}


template <typename ...EVENT>
void Dispatcher<EVENT...>::Unregister(ACallback<EVENT...>& callback)
{
    m_queue.push({false, &callback});
}


template <typename ...EVENT>
void Dispatcher<EVENT...>::Notify(EVENT... event)
{
    UpdateSubs();
    
    std::for_each(m_subs.begin(), m_subs.end(), 
                [event...](ACallback<EVENT...>* iter)
                { 
                    iter->Notify(event...);
                });
}


template <typename ...EVENT>
void Dispatcher<EVENT...>::UpdateSubs()
{
    while(!m_queue.empty())
    {
        std::pair<bool, ACallback<EVENT...>*>& data = m_queue.front(); 
        if(true == data.first)
        {
            m_subs.insert(data.second);
        }
        else
        {
            m_subs.erase(data.second);
        }
        
        m_queue.pop();
    }
}

/////////////////////////////ACallback//////////////////////////////////////
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

/////////////////////////////Callback//////////////////////////////////////
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

#endif //ILRD_RD1645_DISPATCHER_HPP



