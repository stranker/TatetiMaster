#pragma once
#pragma comment (lib, "ws2_32.lib")

#include <WS2tcpip.h>
#include <iostream>

#define VERBOSE false

using namespace std;

class WinSocket
{
public:
	bool init();
	bool stop();
	WinSocket();
	~WinSocket();
};

