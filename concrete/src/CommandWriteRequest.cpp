/********************************
 Name:     Alon Nov
 Exercise: concrete
 Date:	   06.04.2025
 Reviewer: 

********************************/

#include "Handleton.hpp"
#include "MinionManager.hpp"
#include "AMinionArgs.hpp"
#include "CommandWriteRequest.hpp"// API

namespace ilrd
{
namespace details   
{

std::pair<std::function<bool()>, std::chrono::milliseconds> 
    CommandWriteRequest::Run(std::shared_ptr<ITaskArgs> args_)
    {
        Handleton<MinionManager>::GetInstance()->AddWriteTask(args_);
        
        return std::make_pair(nullptr, std::chrono::milliseconds(100)); // ?
    }


} //namespace details
} //namespace ilrd




