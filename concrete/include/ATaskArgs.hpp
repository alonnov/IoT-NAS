/********************************
 Name:     Alon Nov
 Exercise: concrete
 Date:	   06.04.2025
 Reviewer: 

********************************/

#ifndef ILRD_RD1645_ATASKARGS_HPP
#define ILRD_RD1645_ATASKARGS_HPP

#include "utils.hpp"
#include "ITaskArgs.hpp"


namespace ilrd
{
namespace details   
{

class ATaskArgs : public ITaskArgs
{
public:
    ATaskArgs(const UID& uid_);
    virtual const UID& GetUID();

private:
    UID m_uid;
};

class ANBDArgs : public ATaskArgs
{
public:
    ANBDArgs(const UID& uid_, size_t offset_, size_t length_) : ATaskArgs(uid_), m_offset(offset_), m_length(length_){}

    size_t GetOffset()
    {
        return m_offset;
    }

    size_t GetLength()
    {
        return m_length;
    }

    void SetLength(size_t len_)
    {
        m_length = len_;
    }

    void SetOffset(size_t off_)
    {
        m_offset = off_;
    }

private:
    size_t m_offset;
    size_t m_length;
};


class NBDReadArgs : public ANBDArgs
{
public:
    NBDReadArgs(const UID& uid_, size_t offset_, size_t length_) : ANBDArgs(uid_, offset_, length_)
    {}

    int GetKey()
    {
        return READ;
    }
    
    
}; // offset, length (Getters / Setters / ctor)

class NBDWriteArgs : public ANBDArgs
{
public:
    NBDWriteArgs(const UID& uid_, size_t offset_, size_t length_, std::shared_ptr<char[]> str_) : ANBDArgs(uid_, offset_, length_), m_to_write(str_)
    {}
    std::shared_ptr<char[]> GetData()
    {
        return m_to_write;
    }

    int GetKey()
    {
        return WRITE;
    }
        
private:
    std::shared_ptr<char[]> m_to_write;

};


} //namespace details
} //namespace ilrd

#endif //ILRD_RD1645_ATASKARGS_HPP



