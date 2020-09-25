#include "UDPClient.h"

bool UDPClient::connect_to(const char * _ip_address, uint16_t _port) {
	client_socket->open();
	return client_socket->connect_to_host(_ip_address, _port);
}

void UDPClient::send_to(char *data, int data_size, const char* to_ip_address, uint16_t to_port) {
	bool success = client_socket->send_to(data, data_size, to_ip_address, to_port);
	if (!success) {
		if (VERBOSE)cout << "Failed on send bytes" << endl;
	}
	if (VERBOSE)cout << "Success on send bytes" << endl;
}

void UDPClient::send(char * data, int data_size) {
	bool success = client_socket->send_connected(data, data_size);
	if (!success) {
		if (VERBOSE)cout << "Failed on send bytes" << endl;
	}
	if (VERBOSE)cout << "Success on send bytes" << endl;
}

void UDPClient::receive(char * data, int data_size, int & r_read) {
	bool success = client_socket->receive(data, data_size, r_read);
	if (!success) {
		if (VERBOSE)cout << "Server: Failed to receive" << endl;
	}
	if (VERBOSE)cout << "Server: Success to receive" << endl;
}

void UDPClient::process_message(NetworkType command, char* data) {
	switch (command) {
	case CT_CONNECT:
		break;
	case CT_DISCONNECT:
		break;
	case CT_ON_CONNECTED:
		cout << data << endl;
		break;
	default:
		break;
	}
}

void UDPClient::process_packet(NetworkPacket &np) {
	NetworkType  npt = (NetworkType)np.type;
	switch (npt) {
	case NT_CONNECTION:
		_handle_connection_packet(np.data);
		break;
	case NT_GAME:
		_handle_game_packet(np.data);
		break;
	default:
		break;
	}
}

void UDPClient::_handle_connection_packet(char * p) {
	ConnectionPacket *cp = (ConnectionPacket *)p;
	ConnectionType cp_type = (ConnectionType)cp->cmd;
	switch (cp_type) {
	case CT_DISCONNECT:
		break;
	case CT_ON_CONNECTED:
		cout << cp->data << endl;
		break;
	case CT_ON_MATCH:
		cout << cp->data << endl;
	default:
		break;
	}
}

void UDPClient::_handle_game_packet(char * p) {
	GamePacket *cp = (GamePacket *)p;
}

void UDPClient::send_connect() {
	ConnectionPacket cp;
	cout << "Ingresa un alias:";
	cp.cmd = CT_CONNECT;
	cin >> cp.data;
	NetworkPacket p = Packet::create_packet(NT_CONNECTION, (char*)&cp, sizeof(cp));
	send((char*)&p, sizeof(p));
}

bool UDPClient::destroy() {
	if (client_socket) {
		client_socket->close();
		delete client_socket;
	}
	return true;
}

UDPClient::UDPClient() {
	client_socket = new NetSocket();
}


UDPClient::~UDPClient() {
}
