/********************************
 Name:     Alon Nov
 Exercise: concrete
 Date:	   06.04.2025
 Reviewer: 

********************************/

#include "AMinionArgs.hpp"

namespace ilrd
{
namespace details   
{

AMinionArgs::AMinionArgs(const UID& uid_, MasterProxy* master_, size_t offset_, size_t length_) : 
ATaskArgs(uid_), m_offset(offset_), m_length(length_), m_master(master_)
{
}


size_t AMinionArgs::GetOffset()
{
    return m_offset;
}


size_t AMinionArgs::GetLength()
{
    return m_length;
}


MasterProxy& AMinionArgs::GetMasterProxy()
{
    return *m_master;
}


MinionReadArgs::MinionReadArgs(const UID& uid_, MasterProxy* master_, size_t offset_, size_t length_) : AMinionArgs(uid_, master_, offset_, length_)
{
}

int MinionReadArgs::GetKey()
{
    return READ;
}



MinionWriteArgs::MinionWriteArgs(const UID& uid_, MasterProxy* master_, size_t offset_, size_t length_, std::shared_ptr<char[]> to_write_) : AMinionArgs(uid_,  master_, offset_, length_), m_write_data(to_write_)
{
}

int MinionWriteArgs::GetKey()
{
    return WRITE;
}


std::shared_ptr<char[]> MinionWriteArgs::GetData()
{
    return m_write_data;
}


} //namespace details
} //namespace ilrd




