/********************************
 Name:     Alon Nov
 Exercise: concrete
 Date:	   06.04.2025
 Reviewer: 

********************************/

#ifndef ILRD_RD1645_AHPP
#define ILRD_RD1645_AHPP

#include <cstddef>
#include <memory>

#include "UID.hpp"         // UID

namespace ilrd
{
namespace details   
{

enum MESSAGE_TYPE : uint32_t
{
    READ_REQUEST,
    WRITE_REQUEST,
    READ_RESPONSE,
    WRITE_RESPONSE
};


class AMessage
{
public:
    AMessage();
    AMessage(const UID& uid_);
    virtual char* ToBuffer(char* buffer);
    virtual char* FromBuffer(char* buffer);
    virtual uint32_t GetSize();
    const UID& GetUID();
    virtual MESSAGE_TYPE GetClassType() = 0;

private:
    UID m_uid;
};


} //namespace details
} //namespace ilrd

#endif //ILRD_RD1645_AHPP



