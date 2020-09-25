#include <iostream>
#include <string>
#include "UDPServer.h"
#include "UDPClient.h"
#include "GameBase.h"

#pragma comment (lib, "ws2_32.lib")

int main() {

	GameBase *game_base = new GameBase();
	game_base->init();
	game_base->loop();
	game_base->destroy();

	return 0;
}