#pragma once
#pragma comment (lib, "ws2_32.lib")
#include "Helper.h"

#if WINDOWS_ENABLED
#include <WS2tcpip.h>
#define SOCKET_TYPE SOCKET
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>  /* Needed for getaddrinfo() and freeaddrinfo() */
#include <unistd.h> /* Needed for close() */
#define SOCKET_TYPE int
#endif

using namespace std;

class WinSocket
{
public:
	bool init();
	bool stop();
	WinSocket();
	~WinSocket();
};

