/********************************
 Name:     Alon Nov
 Exercise: FD event handling
 Date:	   24.03.2025
 Reviewer: Liad Tiblum

********************************/

#ifndef ILRD_RD1645_ILISTENER_HPP
#define ILRD_RD1645_ILISTENER_HPP

#include <vector>   // std::vector
#include "utils.hpp"

namespace ilrd
{
namespace details
{

class IListener
{
public:

    virtual std::vector<std::pair<int, MODE>> Listen(std::vector<std::pair<int, MODE>>& fds_) = 0;
};


} //namespace details
} //namespace ilrd

#endif //ILRD_RD1645_ILISTENER_HPP



