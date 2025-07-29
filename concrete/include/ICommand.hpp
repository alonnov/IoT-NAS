/********************************
 Name:     Alon Nov
 Exercise: framework
 Date:	   27.03.2025
 Reviewer: 

********************************/

#ifndef ILRD_RD1645_ICOMMAND_HPP
#define ILRD_RD1645_ICOMMAND_HPP

#include <functional>
#include <chrono>

#include "ITaskArgs.hpp"

namespace ilrd
{
namespace details   
{

class ICommand
{
public:
    virtual std::pair<std::function<bool()>, std::chrono::milliseconds> 
        Run(std::shared_ptr<ITaskArgs> args_) = 0;
};

} //namespace details
} //namespace ilrd

#endif //ILRD_RD1645_ICOMMAND_HPP



