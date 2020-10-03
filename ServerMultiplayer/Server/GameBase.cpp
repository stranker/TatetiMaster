#include "GameBase.h"

void GameBase::init() {
	int type;
	do {
		cout << "Server[0] o Cliente[1]:?";
		cin >> type;
	} while (type < 0 || type > 1);
	win_socket->init();
	switch ((UDPType)type) {
	case UDP_SERVER: {
		create_server();
		break;
	}
	case UDP_CLIENT: {
		create_client();
		break;
	}
	default:
		break;
	}
}

void GameBase::create_server() {
	cout << "***Creando server...***" << endl;
	is_server = true;
	udp_server = new UDPServer();
	cout << "Escribe puerto a escuchar:";
	uint16_t port;
	cin >> port;
	if (port == 0) {
		port = 27015;
	}
	udp_server->listen("127.0.0.1", (uint16_t)port);
}

void GameBase::create_client() {
	cout << "*********************************" << endl;
	cout << "*         Tateti Master!        *" << endl;
	cout << "*********************************" << endl;
	cout << "***         O | X | X         ***" << endl;
	cout << "***         X | O | X         ***" << endl;
	cout << "***         X | X | O         ***" << endl;
	cout << "*********************************" << endl;
	is_server = false;
	cout << "Escribe la ip a conectar:";
	string ip_address;
	cin >> ip_address;
	if (ip_address == "0") {
		ip_address = "127.0.0.1";
	}
	cout << "Escribe puerto a conectar:";
	uint16_t port;
	cin >> port;
	if (port == 0) {
		port = 27015;
	}
	udp_client = new UDPClient();
	bool connected = udp_client->connect_to(ip_address.c_str(), port);
	if (connected) {
		udp_client->send_connect();
	}
}

void GameBase::loop() {
	if (is_server) {
		server_loop();
	}
	else {
		client_loop();
	}
}

void GameBase::server_loop() {
	while (true) {
		udp_server->poll_sockets();
	}
}

void GameBase::client_loop() {

	while (!udp_client->queued_for_exit()) {
		udp_client->process_state();
	}
}

void GameBase::destroy() {
	if (udp_server) {
		udp_server->stop();
		delete udp_server;
	}
	if (udp_client) {
		udp_client->destroy();
		delete udp_client;
	}
	if (win_socket) {
		win_socket->stop();
		delete win_socket;
	}
}

GameBase::GameBase() {
	win_socket = new WinSocket();
}


GameBase::~GameBase() {
}
