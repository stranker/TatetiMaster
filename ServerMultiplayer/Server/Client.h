#pragma once

#include <iostream>
#include <string>

using namespace std;

class Client{
private:
	char name[32];
	unsigned int client_port;
	char client_ip[16];
	bool on_match = false;
public:
	void set_data(const char* ip_address, int port);
	void set_name(const char* _name);
	int client_size();
	int ip_size();
	unsigned int get_port();
	const char* get_ip_address();
	const char* get_name();
	bool is_equal_to(Client *other_client);
	bool operator ==(const Client &c) const;
	bool is_on_match() const;
	void set_on_match(bool _on_match);
	Client();
	~Client();
};

