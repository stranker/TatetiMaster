#pragma once
#include "NetSocket.h"
#include "Client.h"
#include "Packet.h"
#include "TatetiMatch.h"
#include <algorithm>
#include <vector>
#include <string>
#include <stdlib.h>
#include <time.h>

class UDPServer{
	int port;
	const char* ip_address;
	bool is_blocking = true;
	NetSocket *server_socket;
	vector<Client*> clients;
	vector<Client*> waiting_clients;
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
	void _notify_client_match(Client * client, string info, int info_size);
	void _set_player_turn(Client *c);
	void _verify_play(byte row, byte column, Client &from_client);
	void _update_match(TatetiMatch *match, char row, char column);
	void _update_match_values(TatetiMatch *match, char row, char column);
	void _set_next_turn(TatetiMatch *match);
	void _on_winner(TatetiMatch *match);
	void _notify_bad_play(Client &from_client);
	UDPServer();
	~UDPServer();
};

