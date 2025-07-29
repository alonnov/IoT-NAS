/********************************
 Name:     Alon Nov
 Exercise: TCP/UDP ping-pong
 Date:	   23.02.2025   
 Reviewer: Zack Molko

********************************/

#include <stdlib.h>				/* atoi	*/
#include <string.h>				/* memset */
#include <stdio.h>				/* puts */
#include <unistd.h>				/* close */
#include <sys/socket.h>			/* socket, bind */
#include <netdb.h>				/* addrinfo, getaddrinfo, freeaddrinfo, AI_PASSIVE*/
#include <assert.h>				/* assert */

#include "udp.h"				/* API */


sockinfo* UDPCreate(const char* ip_addr, const char* port)
{
	sockinfo* sock = SockCreateSocketInfo(ip_addr, port, UDP);
	
	SockCreate(sock);
	if(!sock)
	{
		return NULL;
	}

	return sock;
}


void UDPDestroy(sockinfo* socket)
{
	SockDestroy(socket);
}


ssize_t UDPSendTo(int fd, struct sockaddr* send_addr, void* data, size_t data_size)
{
	ssize_t res = 0;

	assert(data);

	res = sendto(fd, data, data_size, 0, send_addr, sizeof(struct sockaddr));
	if(res == -1)
	{
		// puts("UDPSendTo error");
	}

	return res;
}


ssize_t UDPRecv(int fd, void* data, size_t data_size)
{
	ssize_t res = 0;

	assert(data);

	res = recv(fd, data, data_size, 0);
	if(res == -1)
	{
		// puts("UDPSendTo error");
	}

	return res;
}


ssize_t UDPRecvFrom(int fd, struct sockaddr* send_addr, void* buffer, size_t data_size)
{
	ssize_t res = 0;
	unsigned int size = sizeof(struct sockaddr);

	res = recvfrom(fd, buffer, data_size, 0, send_addr, &size);
	if(-1 == res)
	{
		puts("UDPRecvFrom error");
	}

	return res;
}


int UDPSetBroadcast(sockinfo* sock)
{
	int yes = 1;
	int res = 0;

	assert(sock);
	
	res = setsockopt(SockGetFD(sock), SOL_SOCKET, SO_BROADCAST, &yes, sizeof(int));
	if(res == -1)
	{
		// puts("UDPSetBroadcast error");
	}

	return res;
}
