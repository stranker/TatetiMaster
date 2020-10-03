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
	string info = "LISTEN ON PORT:" + to_string(_port);
	Log::print_verbose(info.c_str());
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
		waiting_clients.push_back(new_client);
		if (VERBOSE) cout << "New client added" << endl;

		// Le respondo
		ConnectionPacket cconn;
		cconn.cmd = CT_ON_CONNECTED;
		memcpy(&cconn.data, "Buscando rival...", sizeof(cconn.data));
		Packet::send_to_network_packet(server_socket, NT_CONNECTION, (char*)&cconn, sizeof(cconn), new_client->get_ip_address(), new_client->get_port());

		for (size_t i = 0; i < waiting_clients.size(); i++) {
			Client* other_client = waiting_clients[i];
			if (new_client != other_client && !other_client->is_on_match()) {
				_create_match(new_client, other_client);
				break;
			}
		}
		break;
	case CT_DISCONNECT:
		ConnectionPacket cdisc;
		cdisc.cmd = CT_DISCONNECT;
		Packet::send_to_network_packet(server_socket, NT_CONNECTION, (char*)&cdisc, sizeof(cdisc), from_client.get_ip_address(), from_client.get_port());
		for (size_t i = 0; i < clients.size(); i++) {
			if (from_client.is_equal_to(clients[i])) {
				clients.erase(clients.begin() + i);
			}
		}
		delete new_client;
		break;
	case CT_RESTART:
		new_client->set_data(from_client.get_ip_address(), from_client.get_port());
		new_client->set_name(cp->data);
		waiting_clients.push_back(new_client);
		// Le respondo
		ConnectionPacket cresp;
		cresp.cmd = CT_ON_CONNECTED;
		memcpy(&cresp.data, "Buscando rival...", sizeof(cresp.data));
		Packet::send_to_network_packet(server_socket, NT_CONNECTION, (char*)&cresp, sizeof(cresp), from_client.get_ip_address(), from_client.get_port());
		//Verifico si hay otro cliente esperando
		for (size_t i = 0; i < waiting_clients.size(); i++) {
			Client* other_client = waiting_clients[i];
			if (new_client != other_client && !other_client->is_on_match()) {
				_create_match(new_client, other_client);
				break;
			}
		}
		break;
	default:
		delete new_client;
		break;
	}
}

void UDPServer::_handle_game_packet(char* p, Client & from_client) {
	GamePacket *gp = (GamePacket*)p;
	GameType gp_type = (GameType)gp->cmd;
	switch (gp_type) {
	case GT_MESSAGE:
		break;
	case GT_PLAY:
		_verify_play(gp->r, gp->c, from_client);
		break;
	default:
		break;
	}
}

void UDPServer::_create_match(Client *p1, Client *p2) {
	TatetiMatch *match = new TatetiMatch();
	matchs.push_back(match);
	match->set_players(p1, p2);
	string notify1 = "Jugando contra:" + string(p2->get_name());
	string notify2 = "Jugando contra:" + string(p1->get_name());
	_notify_client_match(p1, notify1, sizeof(notify1.length()));
	_notify_client_match(p2, notify2, sizeof(notify2.length()));
	int client_turn = rand() % 2;
	_set_player_turn(client_turn == 0 ? p1 : p2);
	match->set_last_player(client_turn == 0 ? p1 : p2);
	p1->set_on_match(true);
	p2->set_on_match(true);
}

void UDPServer::_notify_client_match(Client * client, string info, int info_size) {
	ConnectionPacket cp;
	cp.cmd = CT_ON_MATCH;
	memset(&cp.data, 0, sizeof(cp.data));
	memcpy(&cp.data, info.c_str(), sizeof(cp.data));
	Packet::send_to_network_packet(server_socket, NT_CONNECTION, (char*)&cp, sizeof(cp), client->get_ip_address(), client->get_port());
}

void UDPServer::_set_player_turn(Client * client) {
	ConnectionPacket cp;
	cp.cmd = CT_SET_TURN;
	string notify = "Es tu turno!";
	memset(&cp.data, 0, sizeof(cp.data));
	memcpy(&cp.data, notify.c_str(), sizeof(cp.data));
	Packet::send_to_network_packet(server_socket, NT_CONNECTION, (char*)&cp, sizeof(cp), client->get_ip_address(), client->get_port());
}

void UDPServer::_verify_play(byte row, byte column, Client & from_client) {
	row = row == 0 ? 1 : row;
	column = column == 0 ? 1 : column;
	bool player_found = false;
	for (size_t i = 0; i < matchs.size(); i++) {
		TatetiMatch *match = matchs[i];
		for (size_t j = 0; j < 2; j++) {
			if (!player_found && from_client.is_equal_to(match->get_players()[j])) {
				player_found = true;
			}
		}
		bool can_input = match->check_input(row, column);
		if (can_input) {
			_update_match(match, row, column);
		}
		else {
			_notify_bad_play(from_client);
		}
	}
}

void UDPServer::_update_match(TatetiMatch * match, char row, char column) {
	match->insert_input(row, column);
	if (match->check_win()) { // CHECK IF PLAYER WIN
		_on_winner(match);
	}
	else {
		match->next_turn();
		_update_match_values(match, row, column);
		if (match->get_holes() == 0) {
			_on_draw(match);
		}
		else {
			_set_next_turn(match);
		}
	}
}

void UDPServer::_update_match_values(TatetiMatch * match, char row, char column) {
	GamePacket gp;
	gp.cmd = GT_PLAY_OK;
	gp.r = row;
	gp.c = column;
	gp.turn = match->get_turn();
	gp.holes = match->get_holes();
	for (size_t i = 0; i < 2; i++) {
		Client *player = match->get_players()[i];
		Packet::send_to_network_packet(server_socket, NT_GAME, (char*)&gp, sizeof(gp), player->get_ip_address(), player->get_port());
	}
}

void UDPServer::_set_next_turn(TatetiMatch * match) {
	for (size_t i = 0; i < 2; i++) {
		Client *player = match->get_players()[i];
		if (player != match->get_last_player()) {
			_set_player_turn(player);
			match->set_last_player(player);
			break;
		}
	}
}

void UDPServer::_on_winner(TatetiMatch * match) {
	Client *winner = match->get_last_player();
	GamePacket gp;
	gp.cmd = GT_WIN;
	string win = "* * * Has ganado! * * *";
	memcpy(&gp.aux, win.c_str(), sizeof(gp.aux));
	Packet::send_to_network_packet(server_socket, NT_GAME, (char*)&gp, sizeof(gp), winner->get_ip_address(), winner->get_port());
	for (size_t i = 0; i < 2; i++) {
		if (winner != match->get_players()[i]) {
			GamePacket gp_loser;
			gp_loser.cmd = GT_LOSE;
			string lose = "* * * Has perdido! * * *";
			memcpy(&gp.aux, lose.c_str(), sizeof(gp.aux));
			Packet::send_to_network_packet(server_socket, NT_GAME, (char*)&gp, sizeof(gp), match->get_players()[i]->get_ip_address(), match->get_players()[i]->get_port());
			break;
		}
	}
	for (size_t i = 0; i < matchs.size(); i++) {
		if (matchs[i] == match) {
			matchs.erase(matchs.begin() + i);
			break;
		}
	}
}

void UDPServer::_on_draw(TatetiMatch * match) {
	GamePacket gp;
	gp.cmd = GT_DRAW;
	string draw = "* * * Has Empatado! * * *";
	memcpy(&gp.aux, draw.c_str(), sizeof(gp.aux));
	for (size_t i = 0; i < 2; i++) {
		Client *client = match->get_players()[i];
		Packet::send_to_network_packet(server_socket, NT_GAME, (char*)&gp, sizeof(gp), client->get_ip_address(), client->get_port());
	}
	for (size_t i = 0; i < matchs.size(); i++) {
		if (matchs[i] == match) {
			matchs.erase(matchs.begin() + i);
			break;
		}
	}
}

void UDPServer::_notify_bad_play(Client & from_client) {
	GamePacket gp;
	gp.cmd = GT_PLAY_NOK;
	string bad_play = " Jugada invalida! ";
	memcpy(&gp.aux, bad_play.c_str(), sizeof(gp.aux));
	Packet::send_to_network_packet(server_socket, NT_GAME, (char*)&gp, sizeof(gp), from_client.get_ip_address(), from_client.get_port());
}

void UDPServer::poll_sockets() {
	NetworkPacket np;
	Client client;
	int client_size = sizeof(client);
	int command = -1;

	if (server_socket->poll(POLL_TYPE_BOTH, 0.01)) {
		receive_from((char *)&np, sizeof(NetworkPacket), client);
		process_packet(np, client);
	}
}

UDPServer::UDPServer(){
	server_socket = new NetSocket();
	srand(time(NULL));
}


UDPServer::~UDPServer(){
}
