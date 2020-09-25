#pragma once
#include <iostream>

using namespace std;

enum NetworkType{NT_CONNECTION, NT_GAME};
enum ConnectionType{CT_CONNECT, CT_DISCONNECT, CT_ON_CONNECTED, CT_ON_MATCH};
enum GameType{GT_MESSAGE, GT_PLAY};

struct ConnectionPacket {
	char cmd;
	char data[255];
};

struct GamePacket {
	char row;
	char column;
};

struct NetworkPacket {
	NetworkType type;
	char data[sizeof(ConnectionPacket)];
};

class Packet {
public:
	static NetworkPacket create_packet(NetworkType type, char *data, int data_size);
};