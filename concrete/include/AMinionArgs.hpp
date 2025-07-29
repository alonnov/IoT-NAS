/********************************
 Name:     Alon Nov
 Exercise: concrete
 Date:	   06.04.2025
 Reviewer: 

********************************/

#ifndef ILRD_RD1645_AMINIONARGS_HPP
#define ILRD_RD1645_AMINIONARGS_HPP

#include "MasterProxy.hpp"

namespace ilrd
{
namespace details   
{

class AMinionArgs : public ATaskArgs
{
public:
    AMinionArgs(const UID& uid_, MasterProxy* master_, size_t offset_, size_t length_);
    size_t GetOffset();
    size_t GetLength();
    MasterProxy& GetMasterProxy();

private:
    size_t m_offset;
    size_t m_length;
    MasterProxy* m_master;
};


class MinionReadArgs final : public AMinionArgs
{
public:
    MinionReadArgs(const UID& uid_, MasterProxy* master_, size_t offset_, size_t length_);
    MinionReadArgs(std::shared_ptr<ITaskArgs> other_);
    int GetKey() override;

};


class MinionWriteArgs final : public AMinionArgs
{
public:
    MinionWriteArgs(const UID& uid_, MasterProxy* master_, size_t offset_, size_t length_, std::shared_ptr<char[]> to_write_);
    int GetKey() override;
    std::shared_ptr<char[]> GetData();
    
private:
    std::shared_ptr<char[]> m_write_data;
};

} //namespace details
} //namespace ilrd

#endif //ILRD_RD1645_AMINIONARGS_HPP



