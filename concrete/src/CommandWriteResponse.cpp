/********************************
 Name:     Alon Nov
 Exercise: concrete
 Date:	   06.04.2025
 Reviewer: 

********************************/

#include "Handleton.hpp"
#include "FileManager.hpp"
#include "AMinionArgs.hpp"
#include "CommandWriteResponse.hpp"// API

namespace ilrd
{
namespace details   
{

std::pair<std::function<bool()>, std::chrono::milliseconds> 
    CommandWriteResponse::Run(std::shared_ptr<ITaskArgs> args_)
    {
        auto minion_args = std::static_pointer_cast<MinionWriteArgs>(args_);

        STATUS status = Handleton<FileManager>::GetInstance()->Write(minion_args->GetOffset(), minion_args->GetLength(), minion_args->GetData());
        
        minion_args->GetMasterProxy().SendWriteResponse(minion_args->GetUID(), status);

        return std::make_pair(nullptr, std::chrono::milliseconds(100)); // ?
    }


} //namespace details
} //namespace ilrd




