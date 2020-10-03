#pragma once

#include "WinSocket.h"
#include "Client.h"

using namespace std;

enum PoolType {POLL_TYPE_READ, POLL_TYPE_WRITE, POLL_TYPE_BOTH};

class NetSocket
{
private:
	const char* ip_address;
	int port;
	SOCKET_TYPE net_socket;
public:
	SOCKET_TYPE get_socket();
	bool bind_socket(const char *_ip_address, uint16_t _port);
	bool close();
	bool open();
	bool connect_to_host(const char* _ip_address, uint16_t _port);
	bool receive(char *data, int data_size, int &read_bytes);
	bool receive_from(char* data, int data_size, Client &client_from);
	bool send_to(char *data, int data_size, const char* to_ip_address, uint16_t to_port);
	bool send_connected(char *data, int data_size);
	bool poll(PoolType p_type, int p_timeout);
	NetSocket();
	~NetSocket();
};

