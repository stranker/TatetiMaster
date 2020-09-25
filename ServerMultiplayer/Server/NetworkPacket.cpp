#include "NetworkPacket.h"



void NetworkPacket::set_data(const char * data) {
	for (size_t i = 0; i < 255; i++) {
		if (i == 0) {
			command = (CommandType)data[0];
		}
		else {
			data_buffer[i] = data[i];
		}
	}
}

void NetworkPacket::clear_data() {
	memset(data_buffer, 0, sizeof(data_buffer));
}

CommandType NetworkPacket::get_command() {
	return command;
}

const char * NetworkPacket::get_data() {
	return data_buffer;
}

NetworkPacket::NetworkPacket() {
}


NetworkPacket::~NetworkPacket() {
}
