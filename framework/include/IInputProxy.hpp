/********************************
 Name:     Alon Nov
 Exercise: framework
 Date:	   27.03.2025
 Reviewer: 

********************************/

#ifndef ILRD_RD1645_IINPUTPROXY_HPP
#define ILRD_RD1645_IINPUTPROXY_HPP

#include <memory>

#include "IListener.hpp"
#include "ATaskArgs.hpp"

namespace ilrd
{
namespace details   
{

class IInputProxy
{
public:
    virtual std::shared_ptr<ATaskArgs> GetTaskArgs(int fd_, MODE mode_) = 0;
};

} //namespace details
} //namespace ilrd

#endif //ILRD_RD1645_IINPUTPROXY_HPP



