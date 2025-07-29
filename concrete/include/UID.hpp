/* *****************************
 Name:     Alon Nov
 Exercise: unique identifier 
 Date:	   07.11.2024   
 Reviewer: Liad Tiblum

********************************/

#ifndef _UID_HPP_ 
#define _UID_HPP_

#include <atomic>
#include <sys/types.h> /* size_t, pid_t, time_t */

namespace ilrd
{
namespace details
{

const int IP_SIZE = 4;

class UID
{
public:
    UID();
    UID(char dummy);
    UID(const UID& other_);
    const UID& operator=(const UID& other_);
    bool operator==(const UID& other_) const;
    bool operator<(const UID& other_) const;
    char* FromBuffer(char* buffer_);
    char* ToBuffer(char* buffer_);
    uint32_t IsBad() const;
    uint32_t GetCounter() const
    {
        return m_this_counter;
    }
    
private:
    time_t m_time;
    pid_t m_pid;
    unsigned char m_ip[IP_SIZE];
    uint32_t m_this_counter;
    uint32_t m_bad;
    static std::atomic<uint32_t> m_counter;
    bool GetIP();
};

} // namespace details
} // namespace ilrd


#endif // _UID_HPP_ 
