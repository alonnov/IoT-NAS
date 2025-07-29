/********************************
 Name:     Alon Nov
 Exercise: scheduler
 Date:	   25.03.2025
 Reviewer: Sahar Moalem

********************************/

#ifndef ILRD_RD1645_ISCHEDULERTASK_HPP
#define ILRD_RD1645_ISCHEDULERTASK_HPP

namespace ilrd
{
namespace details
{


class ISchedulerTask
{
public:
    // run the given task
    virtual void Run() = 0;
};


} //namespace details
} //namespace ilrd





#endif //ILRD_RD1645_ISCHEDULERTASK_HPP



