#pragma once

#include "WinSocket.h"
#include "UDPServer.h"
#include "UDPClient.h"

enum UDPType { UDP_SERVER = 0, UDP_CLIENT = 1};

class GameBase {
private:
	WinSocket *win_socket;
	UDPServer *udp_server;
	UDPClient *udp_client;
	bool is_server = false;
public:
	void init();
	void loop();
	void server_loop();
	void client_loop();
	void create_server();
	void create_client();
	void destroy();
	GameBase();
	~GameBase();
};
