/********************************
 Name:     Alon Nov
 Exercise: concrete
 Date:	   06.04.2025
 Reviewer: 

********************************/

#include <stdexcept>
#include <unistd.h>
#include <linux/nbd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include "NBD.hpp"    // API

namespace ilrd
{
namespace details   
{

int NBD::Init(const std::string dev_path_, size_t minion_size, size_t num_minions)
{
	m_fd = open(dev_path_.c_str(), O_RDWR);

    if(-1 == ioctl(m_fd, NBD_SET_SIZE, minion_size * num_minions))
    {
        throw std::runtime_error("ioctl1");
    }
    if(-1 == ioctl(m_fd, NBD_CLEAR_QUE))
    {
        throw std::runtime_error("ioctl2");
    }

    if(-1 == ioctl(m_fd, NBD_CLEAR_SOCK))
    {
        throw std::runtime_error("ioctl3");
    }

    // 2. Init server and client processes to receive NBD messages:
    // - Create connected sockets to be used by server and client:
    int sp[2];
    socketpair(AF_UNIX, SOCK_STREAM, 0, sp);
    // - Create server: Child process
    pid_t pid = fork();
    if(0 == pid)
    {
        close(sp[0]);
        if(-1 == ioctl(m_fd, NBD_SET_SOCK, sp[1]))
        {
            throw std::runtime_error("ioctl4");
        }
        
        if(-1 == ioctl(m_fd, NBD_DO_IT))
        {
            throw std::runtime_error("ioctl5");
        }

        // Now blocked. When done, cleanup and exit:
        if(-1 == ioctl(m_fd, NBD_CLEAR_QUE))
        {
            throw std::runtime_error("ioctl6");
        }

        if(-1 == ioctl(m_fd, NBD_CLEAR_SOCK))
        {
            throw std::runtime_error("ioctl7");
        }
    }
    // - Client: Continue in parent process
    close(sp[1]);

	return sp[0];
}


} //namespace details
} //namespace ilrd

