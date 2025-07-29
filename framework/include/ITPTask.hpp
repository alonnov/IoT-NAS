/********************************
 Name:     Alon Nov
 Exercise: thread pool
 Date:	   25.03.2025
 Reviewer: 

********************************/

#ifndef ILRD_RD1645_ITPTASK_HPP
#define ILRD_RD1645_ITPTASK_HPP

namespace ilrd
{
namespace details
{


class ITPTask
{
public:
    // run the given task
    virtual void Run() = 0;
};


} //namespace details
} //namespace ilrd



#endif //ILRD_RD1645_ITPTASK_HPP



