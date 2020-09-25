#pragma once

#include "NetSocket.h"
#include "Packet.h"

class UDPClient {
private:
	int port;
	const char* ip_address;
	NetSocket *client_socket;
public:
	bool connect_to(const char* _ip_address, uint16_t _port);
	void send_to(char *data, int data_size, const char* to_ip_address, uint16_t to_port);
	void send(char *data, int data_size);
	void receive(char *data, int data_size, int &r_read);
	void process_message(NetworkType command, char* data);
	void process_packet(NetworkPacket &np);
	void _handle_connection_packet(char* connection_packet);
	void _handle_game_packet(char* game_packet);
	void send_connect();
	bool destroy();
	UDPClient();
	virtual ~UDPClient();
};

