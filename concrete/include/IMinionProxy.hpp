/********************************
 Name:     Alon Nov
 Exercise: concrete
 Date:	   06.04.2025
 Reviewer: 

********************************/

#ifndef ILRD_RD1645_IMINIONPROXY_HPP
#define ILRD_RD1645_IMINIONPROXY_HPP

#include <memory>
#include <functional>

#include "utils.hpp"
#include "AMessage.hpp"

namespace ilrd
{
namespace details   
{

struct MinionArgs
{
    MinionArgs(size_t offset_, size_t length_, const UID& uid_, std::shared_ptr<char[]> data_);
    size_t m_offset;
    size_t m_length;
    UID m_uid;
    std::shared_ptr<char[]> m_data;
};

class IMinionProxy
{

public:
    virtual void SendWriteTask(const MinionArgs& args_, std::function<bool(const TaskResult&)> on_done_) = 0;
    virtual void SendReadTask(const MinionArgs& args_, std::function<bool(const TaskResult&)> on_done_) = 0;
    virtual int GetFD() = 0;
    virtual void OnWakeup() = 0;
};


} //namespace details
} //namespace ilrd

#endif //ILRD_RD1645_IMINIONPROXY_HPP



