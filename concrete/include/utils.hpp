/* *****************************
 Name:     Alon Nov
 Exercise: unique identifier 
 Date:	   07.11.2024   
 Reviewer: Liad Tiblum

********************************/

#ifndef ILRD_RD1645_UTILS_HPP
#define ILRD_RD1645_UTILS_HPP

#include <memory> // shared_ptr

#include "UID.hpp"// UID

namespace ilrd
{
namespace details
{

enum MODE : uint32_t
{
    READ,
    WRITE
};

enum STATUS : uint32_t
{
    SUCCESS,
    FAILURE,
    RUNNING 
};

struct TaskResult
{
    TaskResult(const UID& uid_);
    TaskResult(const UID& uid_, STATUS status_, size_t length_, std::shared_ptr<char[]> read_data_);
    UID m_uid;
    STATUS m_status;
    size_t m_length;
    std::shared_ptr<char[]> m_data;
};

} // namespace details
} // namespace ilrd


#endif // ILRD_RD1645_UTILS_HPP 
