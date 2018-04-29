/**
    Lost Heaven Multiplayer
    UDPWrapper.h
	Purpose: simple UDP wrapper for LHMP written by me
	@author Romop5
*/

#ifndef _UDPWRAPPER_H
#define _UDPWRAPPER_H

#ifdef _WIN32
#include <Windows.h>
typedef int socklen_t;
#else
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fcntl.h>
typedef int SOCKET;
#define INVALID_SOCKET (int) (~0)
#define SOCKET_ERROR	-1
#define closesocket close
#endif

enum UDPWrapperErrors
{
	UDP_OK,
	UDP_WINSOCKFAILED,
	UDP_SOCKETFAILED,
	UDP_BINDINGFAILED,
	UDP_BLOCKINGSOCKET
};
struct UDPPacket
{
	struct sockaddr_in sender;
	unsigned int messageLength;
	unsigned char* data;
};

class UDPWrapper
{
public:
	// starts UDP server at @PORT
	unsigned int StartServer(int port);

	// prepare client for communication with hostname:port
	unsigned int StartClient(char* hostname,int port);

	// send data to receiver - ONLY FOR CLIENT
	void SendData(unsigned int length, const void* data);

	// send data to receiver
	void SendData(unsigned int length, const void* data, sockaddr* receiver);

	// receive data
	// if no data is available, returns NULL
	UDPPacket* Receive();

	// a user-friendly way how to delete packet
	static void DellocatePacket(UDPPacket* packet);

	// stop connection and free all resources
	void CleanUP();
private:
	// starts sockets
	unsigned int StartSockets();
	// create socket
	SOCKET	 CreateSocket();
	SOCKET p_socket;

	sockaddr_in server;
};

#endif
