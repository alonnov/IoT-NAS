/********************************
 Name:     Alon Nov
 Exercise: concrete
 Date:	   06.04.2025
 Reviewer: 

********************************/
#include <string.h>

#include "Logger.hpp"
#include "Message.hpp"
#include "utils.hpp"

namespace ilrd
{
namespace details   
{


//////////////////////////// AMessageRequest //////////////////////////////////
AMessageRequest::AMessageRequest()
{
}

AMessageRequest::AMessageRequest(size_t offset_, size_t length_, const UID& uid_) :
AMessage(uid_), m_offset(offset_), m_length(length_) 
{
}


char* AMessageRequest::ToBuffer(char* buffer)
{
    buffer = this->AMessage::ToBuffer(buffer);

    *reinterpret_cast<uint64_t*>(buffer) = m_offset;
    buffer += sizeof(uint64_t);

    *reinterpret_cast<uint32_t*>(buffer) = m_length;
    buffer += sizeof(uint32_t);

    return buffer;
}

char* AMessageRequest::FromBuffer(char* buffer)
{
    buffer = this->AMessage::FromBuffer(buffer);

    m_offset = *reinterpret_cast<uint64_t*>(buffer);
    buffer += sizeof(uint64_t);

    m_length = *reinterpret_cast<uint32_t*>(buffer);
    buffer += sizeof(uint32_t);

    return buffer;
}


uint32_t AMessageRequest::GetSize()
{
    return this->AMessage::GetSize() + sizeof(m_length) + sizeof(m_offset);
}

uint64_t AMessageRequest::GetOffset()
{
    return m_offset;
}

uint32_t AMessageRequest::GetLength()
{
    return m_length;
}


////////////////////////// MessageReadRequest ///////////////////////////////
MessageReadRequest::MessageReadRequest()
{
}

MessageReadRequest::MessageReadRequest(size_t offset_, size_t length_, const UID& uid_) :
AMessageRequest(offset_, length_, uid_)
{
}

MESSAGE_TYPE MessageReadRequest::GetClassType()
{
    return READ_REQUEST;
}


////////////////////////// MessageWriteRequest ///////////////////////////////
MessageWriteRequest::MessageWriteRequest() 
{
}

MessageWriteRequest::MessageWriteRequest(size_t offset_, size_t length_, 
    const UID& uid_, std::shared_ptr<char[]> to_write_) :
AMessageRequest(offset_, length_, uid_), m_to_write(to_write_)
{
}

char* MessageWriteRequest::ToBuffer(char* buffer)
{
    buffer = AMessageRequest::ToBuffer(buffer);
    
    size_t length = GetLength();
    memcpy(buffer, m_to_write.get(), length);
    buffer += length;

    return buffer;
}

char* MessageWriteRequest::FromBuffer(char* buffer)
{
    buffer = this->AMessageRequest::FromBuffer(buffer);

    size_t length = GetLength();
    m_to_write = std::shared_ptr<char[]>(new char[length]);

    memcpy(m_to_write.get(), buffer, length);
    buffer += length;

    return buffer;
}

uint32_t MessageWriteRequest::GetSize()
{
    return this->AMessageRequest::GetSize() + GetLength();
}

MESSAGE_TYPE MessageWriteRequest::GetClassType()
{
    return WRITE_REQUEST;
}

std::shared_ptr<char[]> MessageWriteRequest::GetData()
{
    return m_to_write;
}


///////////////////////////// AMessageResponse /////////////////////////////////
AMessageResponse::AMessageResponse()
{
}

AMessageResponse::AMessageResponse(const UID& uid_, STATUS status_) :
AMessage(uid_), m_status(status_)
{
}

char* AMessageResponse::ToBuffer(char* buffer)
{
    buffer = this->AMessage::ToBuffer(buffer);

    *reinterpret_cast<STATUS*>(buffer) = m_status; 
    buffer += sizeof(uint32_t);

    return buffer;
}

char* AMessageResponse::FromBuffer(char* buffer)
{
    buffer = this->AMessage::FromBuffer(buffer);

    m_status = static_cast<STATUS>(*buffer);
    buffer += sizeof(uint32_t);

    return buffer;
}

uint32_t AMessageResponse::GetSize()
{
    return this->AMessage::GetSize() + sizeof(m_status);
}

STATUS AMessageResponse::GetStatus()
{
    return m_status;
}

///////////////////////////// MessageReadResponse /////////////////////////////
MessageReadResponse::MessageReadResponse()
{
    Handleton<Logger>::GetInstance()->Log("Message: read response created", Logger::DEBUG);
}


MessageReadResponse::MessageReadResponse(const UID& uid_, STATUS status_, size_t length_, 
    std::shared_ptr<char[]> read_data_) :
AMessageResponse(uid_, status_), m_length(length_), m_read_data(read_data_)
{
    Handleton<Logger>::GetInstance()->Log("Message: read response created", Logger::DEBUG);
}

char* MessageReadResponse::ToBuffer(char* buffer)
{
    buffer = this->AMessageResponse::ToBuffer(buffer);

    *reinterpret_cast<uint32_t*>(buffer) = m_length;
    buffer += sizeof(uint32_t);

    memcpy(buffer, m_read_data.get(), m_length);
    buffer += sizeof(m_read_data);

    return buffer;
}

char* MessageReadResponse::FromBuffer(char* buffer)
{
    buffer = this->AMessageResponse::FromBuffer(buffer);

    m_length = *reinterpret_cast<uint32_t*>(buffer);
    buffer += sizeof(uint32_t);

    m_read_data = std::shared_ptr<char[]>(new char[m_length]);
    memcpy(m_read_data.get(), buffer, m_length);
    buffer += sizeof(m_read_data);

    return buffer;
}

uint32_t MessageReadResponse::GetSize()
{
    return this->AMessageResponse::GetSize() + sizeof(uint32_t) + m_length;
}

MESSAGE_TYPE MessageReadResponse::GetClassType()
{
    return READ_RESPONSE;
}

uint32_t MessageReadResponse::GetLength()
{
    return m_length;
}

std::shared_ptr<char[]> MessageReadResponse::GetData()
{
    return m_read_data;
}

///////////////////////////// MessageWriteResponse /////////////////////////////
MessageWriteResponse::MessageWriteResponse()
{
    Handleton<Logger>::GetInstance()->Log("Message: write response created", Logger::DEBUG);
}

MessageWriteResponse::MessageWriteResponse(const UID& uid_, STATUS status_) :
AMessageResponse(uid_, status_)
{
    Handleton<Logger>::GetInstance()->Log("Message: write response created", Logger::DEBUG);
}

MESSAGE_TYPE MessageWriteResponse::GetClassType()
{
    return WRITE_RESPONSE;
}

uint32_t MessageWriteResponse::GetLength()
{
    return 0;
}

std::shared_ptr<char[]> MessageWriteResponse::GetData()
{
    return nullptr;
}

} //namespace details
} //namespace ilrd




