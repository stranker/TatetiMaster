#include "NetSocket.h"

SOCKET_TYPE NetSocket::get_socket(){
	return net_socket;
}

bool NetSocket::bind_socket(const char *_ip_address, uint16_t _port){
	sockaddr_in address;
	address.sin_family = AF_INET;
	inet_pton(AF_INET, _ip_address, &address.sin_addr);
	address.sin_port = htons(_port);

	int bind_result = bind(net_socket, (SOCKADDR *)&address, sizeof(sockaddr_in));
	if (bind_result == SOCKET_ERROR) {
		cout << "Bind function failed with error" << WSAGetLastError() << endl;
		bind_result = close();
		if (bind_result == SOCKET_ERROR)
			cout << "Close socket function failed with error" << WSAGetLastError() << endl;
		WSACleanup();
		return false;
	}
	return true;
}

bool NetSocket::close(){
	if (VERBOSE) cout << "Closing socket" << endl;
	int status;
#if defined(WINDOWS_ENABLED)
	status = shutdown(net_socket, SD_BOTH);
	if (status == 0) { status = closesocket(net_socket);}
#else
	status = shutdown(net_socket, SD_BOTH);
	if (status == 0) { status = close(net_socket); }
#endif
	return status;
}

bool NetSocket::open() {
	if (VERBOSE)cout << "Open socket" << endl;
	net_socket = socket(AF_INET, SOCK_DGRAM, 0);
	if (net_socket == INVALID_SOCKET) {
		cout << "Socket Open function failed with error" << WSAGetLastError() << endl;
	}
	return true;
}

bool NetSocket::connect_to_host(const char * _ip_address, uint16_t _port) {
	if (VERBOSE) cout << "Connecting to host:" << _ip_address << " ,port:" << _port << endl;
	sockaddr_in address;
	address.sin_family = AF_INET;
	inet_pton(AF_INET, _ip_address, &address.sin_addr);
	address.sin_port = htons(_port);

	int err = connect(net_socket, (SOCKADDR*)&address, sizeof(address));
	if (err == SOCKET_ERROR) {
		if (VERBOSE) cout << "Error connecting socket" << endl;
		return false;
	}
	if (VERBOSE) cout << "Connected to server" << endl;
	return true;
}

bool NetSocket::receive(char * data, int data_size, int & read_bytes) {
	read_bytes = recv(net_socket, data, data_size, 0);
	if (read_bytes < 0) {
		if (VERBOSE)cout << "Error on read bytes" << endl;
		return false;
	}
	return true;
}

bool NetSocket::receive_from(char * data, int data_size, Client & client_from) {
	sockaddr_in from;
	socklen_t from_size = sizeof(sockaddr_in);
	memset(&from, 0, from_size);

	int bytes_in = recvfrom(net_socket, data, data_size, 0, (SOCKADDR*)&from, &from_size);

	if (bytes_in < 0) {
		if (VERBOSE)cout << "Error al recibir data" << endl;
		return false;
	}
	char client_ip[16];
	inet_ntop(AF_INET, (uint8_t *)&from.sin_addr, (PSTR)client_ip, sizeof(client_ip));
	int from_port = htons(from.sin_port);
	client_from.set_data((const char *)client_ip, from_port);

	return true;
}

bool NetSocket::send_to(char * data, int data_size, const char* to_ip_address, uint16_t to_port) {
	sockaddr_in address;
	address.sin_family = AF_INET;
	inet_pton(AF_INET, to_ip_address, &address.sin_addr);
	address.sin_port = htons(to_port);

	int sent_bytes = sendto(net_socket, data, data_size, 0, (SOCKADDR*)&address, sizeof(address));

	if (sent_bytes < 0) {
		if (VERBOSE) cout << "Failed to send_to data" << endl;
		return false;
	}

	return true;
}

bool NetSocket::send_connected(char * data, int data_size) {
	int bytes_sent = send(net_socket, data, data_size, 0);
	if (bytes_sent == SOCKET_ERROR) {
		if (VERBOSE) cout << "Send failed:" << WSAGetLastError() << endl;
		return false;
	}
	if (VERBOSE)cout << "Bytes sent:" << bytes_sent << endl;
	return true;
}

bool NetSocket::poll(PoolType p_type, int p_timeout) {
		bool ready = false;
		fd_set rd, wr, ex;
		fd_set *rdp = nullptr;
		fd_set *wrp = nullptr;
		FD_ZERO(&rd);
		FD_ZERO(&wr);
		FD_ZERO(&ex);
		FD_SET(net_socket, &ex);
		struct timeval timeout = { p_timeout, 0 };
		// For blocking operation, pass nullptr  timeout pointer to select.
		struct timeval *tp = nullptr;
		if (p_timeout >= 0) {
			//  If timeout is non-negative, we want to specify the timeout instead.
			tp = &timeout;
		}

		switch (p_type) {
		case POLL_TYPE_READ:
			FD_SET(net_socket, &rd);
			rdp = &rd;
			break;
		case POLL_TYPE_WRITE:
			FD_SET(net_socket, &wr);
			wrp = &wr;
			break;
		case POLL_TYPE_BOTH:
			FD_SET(net_socket, &rd);
			FD_SET(net_socket, &wr);
			rdp = &rd;
			wrp = &wr;
		}
		int ret = select(1, rdp, wrp, &ex, tp);

		if (ret == SOCKET_ERROR) {
			Log::print_verbose("SOCKET ERROR");
			return false;
		}

		if (ret == 0) {
			Log::print_verbose("BUSY");
			return false;
		}

		if (FD_ISSET(net_socket, &ex)) {
			Log::print_verbose("EXCEPTION");
			return false;
		}

		if (rdp && FD_ISSET(net_socket, rdp))
			ready = true;
		if (wrp && FD_ISSET(net_socket, wrp))
			ready = true;
	return ready ? true : false;
}

NetSocket::NetSocket(){
}


NetSocket::~NetSocket(){
}
