/********************************
 Name:     Alon Nov
 Exercise: TCP/UDP ping-pong
 Date:	   23.02.2025   
 Reviewer: Zack Molko

********************************/

#include <stdlib.h>				/* malloc, free */
#include <string.h>				/* memset */
#include <stdio.h>				/* puts */
#include <unistd.h>				/* close */
#include <sys/socket.h>			/* socket, bind */
#include <netdb.h>				/* addrinfo, getaddrinfo, freeaddrinfo, AI_PASSIVE*/

#include "../include/sockets.h"	/* API funcs */

struct sockinfo
{
	struct addrinfo* info;
	int sockfd;
};


sockinfo* SockCreateSocketInfo(const char* ip_addr, const char* port_num, SOCK_TYPE socktype)
{
	struct addrinfo hints = {0};
	sockinfo* sock = (sockinfo*)malloc(sizeof(sockinfo));

	memset(&hints, 0, sizeof(hints));
	hints.ai_flags = AI_PASSIVE;	
	hints.ai_family = AF_INET;
	hints.ai_socktype = socktype;

	if(getaddrinfo(ip_addr, port_num, &hints, &sock->info) != 0)
	{
		// puts("SockGetInfo error");
		return NULL;
	}

	return sock;
}


int SockCreate(sockinfo* sock)
{
	sock->sockfd = socket(sock->info->ai_family, sock->info->ai_socktype, sock->info->ai_protocol);
	if(sock->sockfd == -1)
	{
		// puts("SockCreate error");
		close(sock->sockfd);
		return 1;
	}

	return 0;
}


void SockDestroy(sockinfo* sock)
{
	freeaddrinfo(sock->info);	
	close(sock->sockfd);
	free(sock);
}


int SockBind(sockinfo* sock)
{
	int yes = 1;

	if(setsockopt(SockGetFD(sock), SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
	{
		// puts("SetOp error");
		return 1;
	}

	if(bind(sock->sockfd, SockGetAddr(sock), SockGetLen(sock)) == -1)
	{
		// puts("SockBind error");
		return 1;
	}

	return 0;
}

struct sockaddr* SockGetAddr(sockinfo* sock)
{
	return (struct sockaddr*)(sock->info->ai_addr);
}

unsigned int SockGetLen(sockinfo* sock)
{
	return sock->info->ai_addrlen;
}

int SockGetFD(sockinfo* sock)
{
	return sock->sockfd;
}
