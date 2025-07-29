/********************************
 Name:     Alon Nov
 Exercise: TCP/UDP ping-pong
 Date:	   23.02.2025   
 Reviewer: Zack Molko

********************************/

#ifndef _UDP_H_ 
#define _UDP_H_

#include <sys/types.h>  /* size_t, ssize_t */

#include "sockets.h"


sockinfo* UDPCreate(const char* ipaddr, const char* port);

void UDPDestroy(sockinfo* sock);

ssize_t UDPSendTo(int fd, struct sockaddr* send_addr, void* data, size_t data_size);

ssize_t UDPRecv(int fd, void* data, size_t data_size);

ssize_t UDPRecvFrom(int fd, struct sockaddr* send_addr, void* buffer, size_t data_size);

int UDPSetBroadcast(sockinfo* sock);

#endif /* UDP */