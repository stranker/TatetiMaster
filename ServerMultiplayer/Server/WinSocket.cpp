#include "WinSocket.h"

bool WinSocket::init(){
	Log::print_verbose("WSAS INIT");
#if defined(WINDOWS_ENABLED)
	WSADATA data;
	WORD version = MAKEWORD(2, 2);
	int isOk = WSAStartup(version, &data);
	if (isOk != 0) {
		Log::print_verbose("WSAS START FAILED");
		return false;
	}
#endif
	return true;
}

bool WinSocket::stop(){
	Log::print_verbose("WSAS DESTROY");
#if defined(WINDOWS_ENABLED)
	WSACleanup();
#endif
	return true;
}

WinSocket::WinSocket(){

}


WinSocket::~WinSocket(){
}
