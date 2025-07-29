/********************************
 Name:     Alon Nov
 Exercise: thread pool
 Date:	   10.03.2025   
 Reviewer: Itai Hirschhorn

********************************/

#ifndef ILRD_RD1645_FUNCTIONTASK_HPP
#define ILRD_RD1645_FUNCTIONTASK_HPP

#include <functional>   /* std::function, std::bind */
#include <semaphore>    /* std::binary_semaphore, acquire, release*/

#include "ITPTask.hpp"

namespace ilrd
{
namespace details
{

    
class FunctionTask : public ITPTask
{
public:
    // creates a task
    FunctionTask(std::function<void()>);

    // destroys a task
    ~FunctionTask();

    // run the given task
    void Run();
private:
    std::function<void()> m_func;
};


template <typename OUTPUT>
class FutureTask : public ITPTask
{
public:  
    // creates a task that it's return value is returned with GetOutput (blocks)
    FutureTask(std::function<OUTPUT()>);
    ~FutureTask();  

    // runs the task
    void Run();

    // returns the output of the task
    const OUTPUT& GetOutput();
    
private:
    std::function<OUTPUT()> m_func;
    OUTPUT m_out;
    std::binary_semaphore m_sem;
};

///////////////////////FutureTask//////////////////////////

template <typename OUTPUT>
FutureTask<OUTPUT>::FutureTask(std::function<OUTPUT()> task) : m_func(std::bind(task)), m_sem(0)
{ 
}

template <typename OUTPUT>
FutureTask<OUTPUT>::~FutureTask()
{
}

template <typename OUTPUT>
void FutureTask<OUTPUT>::Run()
{
    m_out = m_func();
    m_sem.release();
}


template <typename OUTPUT>
const OUTPUT& FutureTask<OUTPUT>::GetOutput()
{
    m_sem.acquire();

    return m_out;
}

} //namespace details
} //namespace ilrd





#endif //ILRD_RD1645_FUNCTIONTASK_HPP



