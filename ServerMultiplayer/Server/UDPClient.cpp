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

void UDPClient::_process_packet(NetworkPacket &np) {
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
		current_state = ON_EXIT;
		break;
	case CT_ON_CONNECTED:
		cout << cp->data << endl;
		break;
	case CT_ON_MATCH:
		cout << cp->data << endl;
		current_state = WAIT;
		if (!current_match) {
			current_match = new TatetiMatch();
			current_match->show_board();
		}
		break;
	case CT_SET_TURN:
		cout << cp->data << endl;
		current_state = ON_TURN;
		break;
	default:
		break;
	}
}

void UDPClient::_handle_game_packet(char * p) {
	GamePacket *gp = (GamePacket *)p;
	GameType gp_type = (GameType)gp->cmd;
	switch (gp_type) {
	case GT_MESSAGE:
		break;
	case GT_PLAY:
		break;
	case GT_PLAY_OK:
		_update_board(gp->r, gp->c, gp->turn, gp->holes);
		current_state = WAIT;
		break;
	case GT_PLAY_NOK:
		cout << gp->aux << endl;
		current_state = ON_TURN;
		break;
	case GT_WIN:
		cout << gp->aux << endl;
		current_state = ON_TURN;
		_on_restart();
		break;
	case GT_LOSE:
		cout << gp->aux << endl;
		current_state = ON_TURN;
		_on_restart();
		break;
	default:
		break;
	}
}

void UDPClient::_update_board(char row, char column, char turn, char holes) {
	current_match->insert_input(row, column);
	current_match->set_turn(turn);
	current_match->set_holes(holes);
	current_match->show_board();
}

void UDPClient::_on_restart() {
	cout << "Quieres jugar de nuevo? Si[1] , No[0]" << endl;
	int answer;
	cin >> answer;
	ConnectionPacket cp;
	cp.cmd = answer == 1 ? CT_RESTART : CT_DISCONNECT;
	Packet::send_network_packet(client_socket, NT_CONNECTION, (char*)&cp, sizeof(cp));
	current_state = WAIT;
	delete current_match;
	current_match = NULL;
}

void UDPClient::process_state() {
	NetworkPacket packet;
	int command = -1;
	switch (current_state) {
	case WAIT:
		memset(&packet.data, 0, sizeof(packet.data));
		int r_bytes;
		receive((char*)&packet, sizeof(packet), r_bytes);
		_process_packet(packet);
		break;
	case ON_TURN:
		cout << "Ingresa fila:";
		byte r;
		cin >> r;
		cout << "Ingresa columna:";
		byte c;
		cin >> c;
		GamePacket gp;
		gp.cmd = GT_PLAY;
		gp.c = c;
		gp.r = r;
		memset(&gp.aux, 0, sizeof(gp.aux));
		Packet::send_network_packet(client_socket, NT_GAME, (char*)&gp, sizeof(gp));
		current_state = WAIT;
		break;
	case ON_EXIT:
		queue_exit = true;
		break;
	default:
		break;
	}
}

void UDPClient::send_connect() {
	ConnectionPacket cp;
	cout << "Ingresa un alias:";
	cp.cmd = CT_CONNECT;
	cin >> cp.data;
	Packet::send_network_packet(client_socket, NT_GAME, (char*)&cp, sizeof(cp));
	NetworkPacket p = Packet::create_netwok_packet(NT_CONNECTION, (char*)&cp, sizeof(cp));
	send((char*)&p, sizeof(p));
}

bool UDPClient::destroy() {
	if (client_socket) {
		client_socket->close();
		delete client_socket;
	}
	if (current_match) {
		delete current_match;
	}
	return true;
}

bool UDPClient::is_connected() {
	return connected;
}

bool UDPClient::queued_for_exit() {
	return queue_exit;
}

ClientState UDPClient::get_current_state() const {
	return current_state;
}

UDPClient::UDPClient() {
	client_socket = new NetSocket();
}


UDPClient::~UDPClient() {
}
