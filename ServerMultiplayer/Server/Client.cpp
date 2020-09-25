#include "Client.h"


void Client::set_data(const char* ip_address, int port) {
	memcpy(client_ip, ip_address, ip_size());
	client_port = port;
}

void Client::set_name(const char * _name) {
	memcpy(&name, _name, sizeof(name));
}

int Client::client_size() {
	return (int)sizeof(Client);
}

int Client::ip_size() {
	return sizeof(client_ip);
}

unsigned int Client::get_port() {
	return client_port;
}

const char * Client::get_ip_address() {
	return client_ip;
}

const char* Client::get_name() {
	return name;
}

bool Client::operator==(const Client & c) const {
	return strcmp(client_ip, c.client_ip) == 0 && client_port == c.client_port;
}

Client::Client() {
	memset(&name, 0, sizeof(name));
	memset(&client_ip, 0, sizeof(client_ip));
}


Client::~Client(){
}
