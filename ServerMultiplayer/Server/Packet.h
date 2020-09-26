#pragma once
#include <iostream>
#include "NetSocket.h"

using namespace std;

enum NetworkType{NT_CONNECTION, NT_GAME};
enum ConnectionType{CT_CONNECT, CT_DISCONNECT, CT_ON_CONNECTED, CT_ON_MATCH, CT_SET_TURN, CT_RESTART};
enum GameType{GT_MESSAGE, GT_PLAY, GT_PLAY_OK, GT_PLAY_NOK, GT_WIN, GT_LOSE};

struct ConnectionPacket {
	char cmd;
	char data[255];
};

struct GamePacket {
	byte cmd;
	byte r;
	byte c;
	byte turn;
	byte holes;
	byte aux[250];
};

struct NetworkPacket {
	NetworkType type;
	char data[sizeof(ConnectionPacket)];
};

class Packet {
public:
	static NetworkPacket create_netwok_packet(NetworkType packet_type, char *data, int data_size);
	static void send_network_packet(NetSocket *net_socket, NetworkType packet_type, char *data, int data_size);
	static void send_to_network_packet(NetSocket *net_socket, NetworkType packet_type, char *data, int data_size, const char* ip_address, uint16_t port);
};