/********************************
 Name:     Alon Nov
 Exercise: framework
 Date:	   27.03.2025
 Reviewer: 

********************************/

#ifndef ILRD_RD1645_ITASKARGS_HPP
#define ILRD_RD1645_ITASKARGS_HPP

namespace ilrd
{
namespace details   
{

class ITaskArgs
{
public:
    virtual int GetKey() = 0;
};

} //namespace details
} //namespace ilrd

#endif //ILRD_RD1645_ITASKARGS_HPP



