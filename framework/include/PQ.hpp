/********************************
 Name:     Alon Nov
 Exercise: waitable queue
 Date:	   06.03.2025   
 Reviewer: Mor Weisberg

********************************/

#ifndef ILRD_RD1645_WAITPQ_HPP
#define ILRD_RD1645_WAITPQ_HPP


namespace ilrd
{
namespace details
{

template <typename T>
class PQ
{
public:
    // removes an element at the front of the priority queue
    void pop();

    // inserts an element to its appropriate position in the priority queue
    void push(const T& elem);

    // tests whether the priority queue is empty
    bool empty() const;

    // returns the element at the front of the priority queue
    T front() const;

private:
    std::priority_queue<T> m_queue;

};


template <typename T>
void PQ<T>::pop()
{
    m_queue.pop();
}


template <typename T>
void PQ<T>::push(const T& elem)
{
    m_queue.push(elem);
}


template <typename T>
bool PQ<T>::empty() const
{
    return m_queue.empty();
}


template <typename T>
T PQ<T>::front() const
{
    return m_queue.top();
}

} //namespace details
} //namespace ilrd

#endif //ILRD_RD1645_WAITPQ_HPP



