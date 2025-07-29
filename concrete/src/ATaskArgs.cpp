/********************************
 Name:     Alon Nov
 Exercise: concrete
 Date:	   06.04.2025
 Reviewer: 

********************************/

#include "ATaskArgs.hpp"// API

namespace ilrd
{
namespace details
{

ATaskArgs::ATaskArgs(const UID& uid_) : m_uid(uid_)
{
}

const UID& ATaskArgs::GetUID()
{
    return m_uid;
}


} //namespace details
} //namespace ilrd




