/********************************
 Name:     Alon Nov
 Exercise: concrete
 Date:	   06.04.2025
 Reviewer: 

********************************/

#ifndef ILRD_RD1645_NBD_HPP
#define ILRD_RD1645_NBD_HPP

#include <string> 

namespace ilrd
{
namespace details   
{

class NBD
{
public:
    int Init(const std::string dev_path_, size_t minion_size, size_t num_minions);
    
private:
    int m_fd;

};



} //namespace details
} //namespace ilrd

#endif //ILRD_RD1645_NBD_HPP



