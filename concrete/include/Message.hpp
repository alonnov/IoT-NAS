/********************************
 Name:     Alon Nov
 Exercise: concrete
 Date:	   06.04.2025
 Reviewer: 

********************************/

#ifndef ILRD_RD1645_MESSAGE_HPP
#define ILRD_RD1645_MESSAGE_HPP

#include "utils.hpp"
#include "AMessage.hpp"

namespace ilrd
{
namespace details   
{


class AMessageRequest : public AMessage
{
public:
    AMessageRequest();
    AMessageRequest(size_t offset_, size_t length_, const UID& uid_);
    char* ToBuffer(char* buffer) override;
    char* FromBuffer(char* buffer) override;
    uint32_t GetSize();
    uint32_t GetLength();
    uint64_t GetOffset();

private:
    uint64_t m_offset;
    uint32_t m_length;
};


class MessageReadRequest final : public AMessageRequest
{
public:
    MessageReadRequest();
    MessageReadRequest(size_t offset_, size_t length_, const UID& uid_);
    MESSAGE_TYPE GetClassType();
};


class MessageWriteRequest final : public AMessageRequest
{
public:
    MessageWriteRequest();
    MessageWriteRequest(size_t offset_, size_t length_, const UID& uid_, std::shared_ptr<char[]> to_write_);
    char* ToBuffer(char* buffer) override;
    char* FromBuffer(char* buffer) override;
    uint32_t GetSize();
    MESSAGE_TYPE GetClassType();
    std::shared_ptr<char[]> GetData();

private:
    std::shared_ptr<char[]> m_to_write;
};



class AMessageResponse : public AMessage
{
public:
    AMessageResponse();
    AMessageResponse(const UID& uid_, STATUS status_);
    char* ToBuffer(char* buffer) override;
    char* FromBuffer(char* buffer) override;
    uint32_t GetSize();
    STATUS GetStatus();
    virtual uint32_t GetLength() = 0;
    virtual std::shared_ptr<char[]> GetData() = 0;
    
private:
    STATUS m_status;
};


class MessageReadResponse final : public AMessageResponse
{
public:
    MessageReadResponse();
    MessageReadResponse(const UID& uid_, STATUS status_, size_t length_, std::shared_ptr<char[]> read_data_);
    char* ToBuffer(char* buffer) override;
    char* FromBuffer(char* buffer) override;
    uint32_t GetSize();
    MESSAGE_TYPE GetClassType();
    uint32_t GetLength();
    std::shared_ptr<char[]> GetData();

private:
    uint32_t m_length;
    std::shared_ptr<char[]> m_read_data;
};


class MessageWriteResponse final : public AMessageResponse
{
public:
    MessageWriteResponse();
    MessageWriteResponse(const UID& uid_, STATUS status_);
    MESSAGE_TYPE GetClassType();
    uint32_t GetLength();
    std::shared_ptr<char[]> GetData();
};


} //namespace details
} //namespace ilrd

#endif //ILRD_RD1645_MESSAGE_HPP



