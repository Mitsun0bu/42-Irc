#include "../incs/main.hpp"

int setServerSocket(int port)
{
	//	ALLOCATE A SOCKET FILE DESCRIPTOR (endpoint for communication)	//
	//		int socket(int domain, int type, int protocol);				//
	int	serverSocket = socket(AF_INET, SOCK_STREAM, 0);


	//	SET THE ADDRESS OF THE SOCKET	//
	struct sockaddr_in	serverSocketAddr;
	const char*			internetHostAddress = "127.0.0.1";
	setServerSocketAddr(serverSocketAddr, internetHostAddress, port);


	// BIND THE serverSocket TO serverSocketAddr //
	if (bindServerSocket(serverSocket, serverSocketAddr) == FAILED)
		return (FAILED);


	return (serverSocket);
}