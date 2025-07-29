/********************************
 Name:     Alon Nov
 Exercise: TCP/UDP ping-pong
 Date:	   23.02.2025   
 Reviewer: Zack Molko

********************************/

#ifndef _SOCKETS_H_ 
#define _SOCKETS_H_

#include <sys/socket.h>	/* SOCK_STREAM, SOCK_DGRAM */

typedef struct sockinfo sockinfo;

typedef enum SOCKS
{
	TCP = SOCK_STREAM,    
	UDP = SOCK_DGRAM    
}SOCK_TYPE;

sockinfo* SockCreateSocketInfo(const char* ip_addr, const char* port_num, SOCK_TYPE socktype);

int SockCreate(sockinfo* sock);

void SockDestroy(sockinfo* socket);

int SockBind(sockinfo* socket);

struct sockaddr* SockGetAddr(sockinfo* sock);

unsigned int SockGetLen(sockinfo* sock);

int SockGetFD(sockinfo* socket);

#endif /* SOCKETS */