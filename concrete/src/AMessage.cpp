/********************************
 Name:     Alon Nov
 Exercise: concrete
 Date:	   06.04.2025
 Reviewer: 

********************************/
#include <string.h>

#include "Handleton.hpp"
#include "ResponseManager.hpp"
#include "MinionManager.hpp"// API

namespace ilrd
{
namespace details   
{

//////////////////////////////// AMessage /////////////////////////////////////
AMessage::AMessage() : m_uid('a')
{
}

AMessage::AMessage(const UID& uid_) : m_uid(uid_)
{
}


char* AMessage::ToBuffer(char* buffer)
{
    *reinterpret_cast<uint32_t*>(buffer) = GetSize();
    buffer += sizeof(uint32_t);

    *reinterpret_cast<MESSAGE_TYPE*>(buffer) = GetClassType();
    buffer += sizeof(MESSAGE_TYPE);

    *reinterpret_cast<UID*>(buffer) = GetUID();
    buffer += sizeof(UID);

    return buffer;
}

char* AMessage::FromBuffer(char* buffer)
{
    buffer += 2 * sizeof(uint32_t);

    m_uid = *reinterpret_cast<UID*>(buffer);
    buffer += sizeof(UID);

    return buffer;
}

uint32_t AMessage::GetSize()
{
    return sizeof(m_uid) + sizeof(uint32_t) * 2;
}

const UID& AMessage::GetUID()
{
    return m_uid;
}


} //namespace details
} //namespace ilrd




