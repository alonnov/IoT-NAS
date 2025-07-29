/* *****************************
 Name:     Alon Nov
 Exercise: unique identifier 
 Date:	   07.11.2024   
 Reviewer: Liad Tiblum

********************************/

#include "utils.hpp"   /* API */

namespace ilrd
{
namespace details   
{


TaskResult::TaskResult(const UID& uid_, STATUS status_, size_t length_, std::shared_ptr<char[]> read_data_) :
m_uid(uid_), m_status(status_), m_length(length_), m_data(read_data_)
{
}

TaskResult::TaskResult(const UID& uid_) : m_uid(uid_), m_status(RUNNING), m_length(0), m_data(nullptr)
{
}


} //namespace details
} //namespace ilrd