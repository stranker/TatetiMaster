#include "WinSocket.h"

bool WinSocket::init(){
	if (VERBOSE) cout << "WSAS INIT" << endl;
	WSADATA data;
	WORD version = MAKEWORD(2, 2);
	int isOk = WSAStartup(version, &data);
	if (isOk != 0) {
		if (VERBOSE) cout << "WSAS START FAILED" << endl;
		return false;
	}
	return true;
}

bool WinSocket::stop(){
	if (VERBOSE) cout << "WSAS DESTROY" << endl;
	WSACleanup();
	return true;
}

WinSocket::WinSocket(){

}


WinSocket::~WinSocket(){
}
