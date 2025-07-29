/********************************
 Name:     Alon Nov
 Exercise: concrete
 Date:	   06.04.2025
 Reviewer: 

********************************/

#include <iostream>
#include <cstring>				// memcpy
#include <netinet/in.h>			// ntohl, be64toh
#include <cassert>

#include "Factory.hpp"
#include "Handleton.hpp"
#include "Reactor.hpp"
#include "CommandReadRequest.hpp"
#include "CommandWriteRequest.hpp"
#include "NBDProxy.hpp"    		// API

namespace ilrd
{
namespace details   
{
	

NBDProxy::NBDProxy(int fd_) : m_fd(fd_)
{
}

std::shared_ptr<ATaskArgs> NBDProxy::GetTaskArgs(int fd_, MODE mode_)
{
	(void)mode_;
	(void)fd_;

	std::unique_lock<std::mutex> lock(m_mtx);

	nbd_request request;
	ReadAll(reinterpret_cast<char*>(&request), sizeof(request));
	UID uid;
	m_requests[uid] = request;

	uint32_t len = ntohl(request.len);
	uint64_t from = be64toh(request.from);

	if(ntohl(request.type) == NBD_CMD_READ)
	{
		return std::make_shared<NBDReadArgs>(uid, from, len);
	}

	std::shared_ptr<char[]> chunk = std::shared_ptr<char[]>(new char[len]);
	ReadAll(chunk.get(), len);
	
	return std::make_shared<NBDWriteArgs>(uid, from, len, chunk);
}


void NBDProxy::Register(ACallback<const TaskResult&>* to_reg_)
{
    m_dispatcher.Register(*to_reg_);
}


void NBDProxy::OnTaskDone(const TaskResult& result_)
{
	std::unique_lock<std::mutex> lock(m_mtx);

	nbd_request request = m_requests[result_.m_uid];

	// std::cout << "NBDProxy OnTaskDone: " << std::endl;
	// for(auto& i : m_requests)
	// {
	// 	std::cout << i.first.GetCounter() << ", ";
	// }
	// std::cout << std::endl;

	nbd_reply reply = {htonl(NBD_REPLY_MAGIC), htonl(result_.m_status), request.cookie};
	reply.error = htonl(result_.m_status);

	WriteAll(reinterpret_cast<char*>(&reply), sizeof(reply));

	if(ntohl(request.type) == NBD_CMD_READ)
	{
		WriteAll(result_.m_data.get(), ntohl(request.len));
	}
	m_requests.erase(result_.m_uid);
}


void NBDProxy::ReadAll(char* buffer_, size_t count_)
{
	while(count_ > 0) 
	{
		int bytes_read = read(m_fd, buffer_, count_);
		assert(bytes_read > 0);

		buffer_ += bytes_read;
		count_ -= bytes_read;
	}

	assert(count_ == 0);
}


void NBDProxy::WriteAll(char* buffer_, size_t count_)
{
	while(count_ > 0) 
	{
		int bytes_written = write(m_fd, buffer_, count_);
		assert(bytes_written > 0);

		buffer_ += bytes_written;
		count_ -= bytes_written;
	}

	assert(count_ == 0);
}


} //namespace details
} //namespace ilrd




