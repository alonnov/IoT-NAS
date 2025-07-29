/* *****************************
 Name:     Alon Nov
 Exercise: unique identifier 
 Date:	   06.04.2025   
 Reviewer: 

********************************/

#include <stdio.h>      /* size_t, puts */
#include <string.h>		/* memcmp, memcpy, strcmp */
#include <unistd.h>     /* getpid */
#include <ifaddrs.h>	/* getifaddrs, freeifaddrs, ifaddrs */
#include <netinet/in.h>	/* sockaddr_in, AF_INET */
#include <stdatomic.h> 	/* atomic_size_t, atomic_fetch_add */
#include <time.h>

#include "Handleton.hpp"
#include "Logger.hpp"
#include "UID.hpp"   /* API */

namespace ilrd
{
namespace details   
{

std::atomic<uint32_t> UID::m_counter = 0;

UID::UID() : 
m_time(time(NULL)), m_pid(getpid()), m_this_counter(m_counter++), m_bad(GetIP())
{	
    std::ostringstream str;
    str << "UID(" << m_this_counter << ", " << m_pid << ") created";

    Handleton<Logger>::GetInstance()->Log(str.str(), Logger::DEBUG);
}	


UID::UID(char dummy)
{
    (void)dummy;
}


const UID& UID::operator=(const UID& other_)
{
    m_time = other_.m_time;
    m_pid = other_.m_pid;
    m_this_counter = other_.m_this_counter;
    m_bad = other_.m_bad;
    memcpy(m_ip, other_.m_ip, IP_SIZE);

    return *this;
}

UID::UID(const UID& other_)
{
    m_time = other_.m_time;
    m_pid = other_.m_pid;
    m_this_counter = other_.m_this_counter;
    m_bad = other_.m_bad;
    memcpy(m_ip, other_.m_ip, IP_SIZE);
}

bool UID::operator==(const UID& other_) const
{
	return m_this_counter == other_.m_this_counter &&
            m_time == other_.m_time && 
		    m_pid == other_.m_pid &&
		    0 == memcmp(m_ip, other_.m_ip, IP_SIZE);
}	

bool UID::operator<(const UID& other_) const
{
    return m_this_counter < other_.m_this_counter || m_time < other_.m_time;
}

char* UID::FromBuffer(char* buffer_)
{
    m_time = *reinterpret_cast<time_t*>(buffer_);
    buffer_ += sizeof(time_t);

    m_pid = *reinterpret_cast<pid_t*>(buffer_);
    buffer_ += sizeof(pid_t);

    memcpy(buffer_, m_ip, IP_SIZE);
    buffer_ += IP_SIZE;

    m_this_counter = *reinterpret_cast<uint32_t*>(buffer_);
    buffer_ += sizeof(uint32_t);
    
    m_bad = *reinterpret_cast<uint32_t*>(buffer_);
    buffer_ += sizeof(uint32_t);

    return buffer_;
}


char* UID::ToBuffer(char* buffer_)
{
    *reinterpret_cast<time_t*>(buffer_) = m_time;
    buffer_ += sizeof(time_t);

    *reinterpret_cast<pid_t*>(buffer_) = m_pid;
    buffer_ += sizeof(pid_t);

    memcpy(m_ip, buffer_, IP_SIZE);
    buffer_ += IP_SIZE;

    *reinterpret_cast<uint32_t*>(buffer_) = m_this_counter;
    buffer_ += sizeof(uint32_t);
    
    *reinterpret_cast<uint32_t*>(buffer_) = m_bad;
    buffer_ += sizeof(uint32_t);

    return buffer_;
}


uint32_t UID::IsBad() const
{
    return m_bad;
}


bool UID::GetIP()
{
	struct ifaddrs* list = NULL; 
	struct ifaddrs* runner = NULL;
	struct sockaddr_in* addr = NULL;
	int found = 0;
	
    if(getifaddrs(&list) == -1) 
    {
        Handleton<Logger>::GetInstance()->Log("could not retrieve IP", Logger::ERROR);
        return 1;
    }
	
    for(runner = list; runner != NULL; runner = runner->ifa_next) 
    {
        if(runner->ifa_addr != NULL && runner->ifa_addr->sa_family == AF_INET) 
        {
            addr = (struct sockaddr_in*)runner->ifa_addr;
            
            if(0 != strcmp(runner->ifa_name, "lo"))
            {       
        		memcpy(m_ip, &addr->sin_addr, IP_SIZE); 
        		found = 1; 
        		break;
        	}
        }
    }
    
    freeifaddrs(list);
    if (!found)
    {
        Handleton<Logger>::GetInstance()->Log("could not retrieve IP", Logger::ERROR);
        return 1;
    }
    
    return 0;
}


} //namespace details
} //namespace ilrd