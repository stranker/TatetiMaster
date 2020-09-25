#include "UDPServer.h"

bool UDPServer::stop(){
	if (server_socket) {
		server_socket->close();
		delete server_socket;
	}
	return true;
}

void UDPServer::listen(const char* _ip_address, uint16_t _port) {
	int err = server_socket->open();
	err = server_socket->bind_socket(_ip_address, _port);
	ip_address = _ip_address;
	port = _port;
	cout << "Listen on port:" << port << endl;
}

void UDPServer::receive(char * data, int data_size, int & r_read) {
	bool success = server_socket->receive(data, data_size, r_read);
	if (!success) {
		if (VERBOSE)cout << "Server: Failed to receive" << endl;
	}
	if (VERBOSE)cout << "Server: Success to receive" << endl;
}

void UDPServer::receive_from(char * data, int data_size, Client &from_client) {
	bool success = server_socket->receive_from(data, data_size, from_client);
	if (!success) {
		if (VERBOSE)cout << "Server: Failed on receive_from" << endl;
	}
	if (VERBOSE)cout << "Server: Success on receive_from" << endl;
}

void UDPServer::send_to(char * data, int data_size, Client & from_client) {
	bool success = server_socket->send_to(data, data_size, from_client.get_ip_address(), from_client.get_port());
	if (!success) {
		if (VERBOSE)cout << "Server: Failed on receive_from" << endl;
	}
	if (VERBOSE)cout << "Server: Success on receive_from" << endl;
}

void UDPServer::process_packet(NetworkPacket &np, Client & from_client) {
	NetworkType npt = np.type;
	switch (npt) {
	case NT_CONNECTION:
		_handle_connection_packet(np.data, from_client);
		break;
	case NT_GAME:
		_handle_game_packet(np.data, from_client);
		break;
	default:
		break;
	}
}

void UDPServer::_handle_connection_packet(char* p, Client & from_client) {
	ConnectionPacket *cp = (ConnectionPacket*)p;
	ConnectionType cp_type = (ConnectionType)cp->cmd;

	string alias;
	Client *new_client = new Client();
	switch (cp_type) {
	case CT_CONNECT:
		// Agrego cliente
		alias = string((char *)cp->data);
		alias.erase(remove_if(alias.begin(), alias.end(), isspace), alias.end());
		new_client->set_data(from_client.get_ip_address(), from_client.get_port());
		new_client->set_name(alias.c_str());
		clients.push_back(new_client);
		if (VERBOSE) cout << "New client added" << endl;

		// Le respondo
		ConnectionPacket cresp;
		cresp.cmd = CT_ON_CONNECTED;
		memcpy(&cresp.data, "Buscando rival...", sizeof(cresp.data));
		NetworkPacket resp = Packet::create_packet(NT_CONNECTION, (char*)&cresp, sizeof(cresp));
		server_socket->send_to((char *)&resp, sizeof(resp), new_client->get_ip_address(), new_client->get_port());

		//Verifico si hay otro cliente esperando
		if (clients.size() % 2 == 0) {
			Client *p1 = clients[clients.size() - 2];
			Client *p2 = clients[clients.size() - 1];
			_create_match(p1, p2);
		}
		break;
	case CT_DISCONNECT:
		delete new_client;
		break;
	default:
		delete new_client;
		break;
	}
}

void UDPServer::_handle_game_packet(char* p, Client & from_client) {
	GamePacket *gp = (GamePacket*)p;
	cout << "Row:" << gp->row << endl;
	cout << "Column:" << gp->column << endl;
}

void UDPServer::_create_match(Client *p1, Client *p2) {
	TatetiMatch *match = new TatetiMatch();
	matchs.push_back(match);
	match->set_players(p1, p2);
	ConnectionPacket cp1;
	ConnectionPacket cp2;
	cp1.cmd = CT_ON_MATCH;
	cp2.cmd = CT_ON_MATCH;
	string match_data_1 = "Jugando contra:" + string(p2->get_name());
	string match_data_2 = "Jugando contra:" + string(p1->get_name());
	memcpy(&cp1.data, match_data_1.c_str(), sizeof(cp1.data));
	memcpy(&cp2.data, match_data_2.c_str(), sizeof(cp2.data));
	NetworkPacket np1 = Packet::create_packet(NT_CONNECTION, (char*)&cp1, sizeof(cp1));
	NetworkPacket np2 = Packet::create_packet(NT_CONNECTION, (char*)&cp2, sizeof(cp2));
	server_socket->send_to((char *)&np1, sizeof(NetworkPacket), p1->get_ip_address(), p1->get_port());
	server_socket->send_to((char *)&np2, sizeof(NetworkPacket), p2->get_ip_address(), p2->get_port());
}

UDPServer::UDPServer(){
	server_socket = new NetSocket();
}


UDPServer::~UDPServer(){
}
