/********************************
 Name:     Alon Nov
 Exercise: framework
 Date:	   27.03.2025
 Reviewer: 

********************************/

#ifndef ILRD_RD1645_COMMANDREADREQUEST_HPP
#define ILRD_RD1645_COMMANDREADREQUEST_HPP

#include <functional>
#include <chrono>

#include "ICommand.hpp"

namespace ilrd
{
namespace details   
{

class CommandReadRequest : public ICommand
{
public:
    std::pair<std::function<bool()>, std::chrono::milliseconds> 
        Run(std::shared_ptr<ITaskArgs> args_);
};

} //namespace details
} //namespace ilrd

#endif //ILRD_RD1645_COMMANDREADREQUEST_HPP



