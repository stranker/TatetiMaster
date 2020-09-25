#pragma once
#include "Packet.h"

NetworkPacket Packet::create_packet(NetworkType type, char *data, int data_size) {
	NetworkPacket np;
	np.type = type;
	memcpy(&np.data, data, data_size);
	return np;
}
