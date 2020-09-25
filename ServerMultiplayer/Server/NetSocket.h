#pragma once

#include "WinSocket.h"
#include "Client.h"

using namespace std;

class NetSocket
{
private:
	const char* ip_address;
	int port;
	SOCKET net_socket;
public:
	SOCKET get_socket();
	bool bind_socket(const char *_ip_address, uint16_t _port);
	bool close();
	bool open();
	bool connect_to_host(const char* _ip_address, uint16_t _port);
	bool receive(char *data, int data_size, int &read_bytes);
	bool receive_from(char* data, int data_size, Client &client_from);
	bool send_to(char *data, int data_size, const char* to_ip_address, uint16_t to_port);
	bool send_connected(char *data, int data_size);
	NetSocket();
	~NetSocket();
};

