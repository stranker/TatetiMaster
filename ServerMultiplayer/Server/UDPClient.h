#pragma once

#include "NetSocket.h"
#include "Packet.h"
#include "TatetiMatch.h"

enum ClientState{WAIT, ON_TURN, ON_EXIT};

class UDPClient {
private:
	NetSocket *client_socket;
	ClientState current_state;
	TatetiMatch *current_match;
	bool connected = false;
	bool queue_exit = false;
	string alias;
public:
	bool connect_to(const char* _ip_address, uint16_t _port);
	void send_to(char *data, int data_size, const char* to_ip_address, uint16_t to_port);
	void send(char *data, int data_size);
	void receive(char *data, int data_size, int &r_read);
	void _process_packet(NetworkPacket &np);
	void _handle_connection_packet(char* connection_packet);
	void _handle_game_packet(char* game_packet);
	void _update_board(char row, char column, char turn, char holes);
	void _on_restart();
	void process_state();
	void send_connect();
	bool destroy();
	bool is_connected();
	bool queued_for_exit();
	ClientState get_current_state() const;
	UDPClient();
	virtual ~UDPClient();
};
