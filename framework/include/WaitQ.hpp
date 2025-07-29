/********************************
 Name:     Alon Nov
 Exercise: waitable queue
 Date:	   06.03.2025   
 Reviewer: Mor Weisberg

********************************/

#ifndef ILRD_RD1645_WAITQ_HPP
#define ILRD_RD1645_WAITQ_HPP

#include <mutex>
#include <queue>
#include <condition_variable>
#include <chrono>

namespace ilrd
{
namespace details
{
template <typename T, class Q = std::queue<T>>
class WaitQ
{
public:
    // removes the element at the front of the queue (blocks)
    void Pop(T& out);

    // removes the element at the front of the queue unless "timeout" is reached
    bool Pop(std::chrono::milliseconds timeout, T& out);

    // inserts and element to the back of the queue
    void Push(const T& elem);

    // tests whether the queue is empty
    bool IsEmpty() const;

private:
    Q m_queue;
    mutable std::timed_mutex m_mtx;
    std::condition_variable_any m_cond;
};
    
template <typename T, typename Q>
void WaitQ<T, Q>::Pop(T& out)
{
    std::unique_lock<std::timed_mutex> lock(m_mtx);
        m_cond.wait(lock, 
                    [this]()
                    { 
                        return !m_queue.empty(); 
                    });
        out = m_queue.front();
        m_queue.pop();
        m_cond.notify_one();
}


template <typename T, typename Q>
bool WaitQ<T, Q>::Pop(std::chrono::milliseconds timeout, T& out)
{
    bool stat;

    {
    std::unique_lock<std::timed_mutex> lock(m_mtx, timeout);
        if(true == (stat = m_cond.wait_until(lock, std::chrono::system_clock::now() + timeout, 
                            [this]()
                            {
                                return !m_queue.empty(); 
                            })))
        {
            out = m_queue.front();
            m_queue.pop();
        }     
        m_cond.notify_one(); 
    }
    
    return stat;
}


template <typename T, typename Q>
void WaitQ<T, Q>::Push(const T& elem)
{
    std::unique_lock<std::timed_mutex> lock(m_mtx);
        m_queue.push(elem);
        m_cond.notify_one();
}


template <typename T, typename Q>
bool WaitQ<T, Q>::IsEmpty() const
{
    std::unique_lock<std::timed_mutex> lock(m_mtx);

    return m_queue.empty();
}

} //namespace details
} //namespace ilrd

#endif //ILRD_RD1645_WAITQ_HPP



