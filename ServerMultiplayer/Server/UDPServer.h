#pragma once
#include "NetSocket.h"
#include "Client.h"
#include "Packet.h"
#include "TatetiMatch.h"
#include <algorithm>
#include <vector>
#include <string>

class UDPServer{
	int port;
	const char* ip_address;
	bool is_blocking = true;
	NetSocket *server_socket;
	vector<Client*> clients;
	vector<TatetiMatch*> matchs;
public:
	bool stop();
	void listen(const char* ip_address, uint16_t port);
	void receive(char *data, int data_size, int &r_read);
	void receive_from(char * data, int data_size, Client &from_client);
	void send_to(char *data, int data_size, Client &from_client);
	void process_packet(NetworkPacket &np, Client &from_client);
	void _handle_connection_packet(char* cp, Client &from_client);
	void _handle_game_packet(char* gp, Client &from_client);
	void _create_match(Client *p1, Client *p2);
	UDPServer();
	~UDPServer();
};

