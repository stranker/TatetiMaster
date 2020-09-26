#pragma once
#include "Packet.h"

NetworkPacket Packet::create_netwok_packet(NetworkType packet_type, char * data, int data_size) {
	NetworkPacket packet;
	packet.type = packet_type;
	memcpy(&packet.data, data, data_size);
	return packet;
}

void Packet::send_network_packet(NetSocket * net_socket, NetworkType packet_type, char * data, int data_size) {
	NetworkPacket packet;
	packet.type = packet_type;
	memcpy(&packet.data, data, data_size);
	net_socket->send_connected((char*)&packet, sizeof(packet));
}

void Packet::send_to_network_packet(NetSocket * net_socket, NetworkType packet_type, char * data, int data_size, const char * ip_address, uint16_t port) {
	NetworkPacket packet;
	packet.type = packet_type;
	memcpy(&packet.data, data, data_size);
	net_socket->send_to((char*)&packet, sizeof(packet), ip_address, port);
}
