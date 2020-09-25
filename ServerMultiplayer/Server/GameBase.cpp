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
	udp_client = new UDPClient();
	cout << "Escribe la ip a conectar:";
	string ip_address;
	cin >> ip_address;
	cout << "Escribe puerto a conectar:";
	uint16_t port;
	cin >> port;
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
	NetworkPacket np;
	Client client;
	int client_size = sizeof(client);

	while (true) {
		int command = -1;

		memset(&client, 0, client_size);
		memset(&np.data, 0, sizeof(np.data));

		udp_server->receive_from((char *)&np, sizeof(NetworkPacket), client);

		udp_server->process_packet(np, client);
	}
}

void GameBase::client_loop() {
	NetworkPacket packet;
	int command = -1;
	while (true) {
		memset(&packet.data, 0, sizeof(packet.data));
		int r_bytes;

		udp_client->receive((char*)&packet, sizeof(packet), r_bytes);

		udp_client->process_packet(packet);
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
