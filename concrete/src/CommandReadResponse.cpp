/********************************
 Name:     Alon Nov
 Exercise: concrete
 Date:	   06.04.2025
 Reviewer: 

********************************/

#include "Handleton.hpp"
#include "FileManager.hpp"
#include "AMinionArgs.hpp"
#include "CommandReadResponse.hpp"// API

namespace ilrd
{
namespace details   
{

std::pair<std::function<bool()>, std::chrono::milliseconds> 
    CommandReadResponse::Run(std::shared_ptr<ITaskArgs> args_)
    { 
        auto minion_args = std::static_pointer_cast<MinionReadArgs>(args_);

        std::shared_ptr<char[]> buffer = std::make_shared<char[]>(minion_args->GetLength());
        
        STATUS status = Handleton<FileManager>::GetInstance()->Read(minion_args->GetOffset(), minion_args->GetLength(), buffer);

        minion_args->GetMasterProxy().SendReadResponse(minion_args->GetUID(), status, minion_args->GetLength(), buffer);

        return std::make_pair(nullptr, std::chrono::milliseconds(100)); // ?
    }



} //namespace details
} //namespace ilrd




