/********************************
 Name:     Alon Nov
 Exercise: concrete
 Date:	   06.04.2025
 Reviewer: 

********************************/

#ifndef ILRD_RD1645_IFILEMANAGER_HPP
#define ILRD_RD1645_IFILEMANAGER_HPP

#include "utils.hpp"    // STATUS, shared_ptr

namespace ilrd
{
namespace details   
{


class IFileManager
{
public:
    virtual STATUS Read(size_t offset_, size_t length_, std::shared_ptr<char[]> buffer_) = 0;
    virtual STATUS Write(size_t offset_, size_t length_, std::shared_ptr<char[]> buffer_) = 0;
};


} //namespace details
} //namespace ilrd

#endif //ILRD_RD1645_IFILEMANAGER_HPP



