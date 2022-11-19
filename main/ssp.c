/*
 * spp.c
 *
 * Created: 21/6/2020 15:18
 *  Author: Luis Pichio | luispichio@sistemasindustriales.com | luispichio@gmail.com
 */

#include "ssp.h"

//-----------------------------------------------------------------------------
bool ssp_send_frame(t_spp *spp, int socket, uint16_t type, uint8_t *frame, uint16_t size){
	uint8_t frame_buffer[size + 32];
	uint16_t msg_id = 0;
	size_t _size = 0;
	memcpy(frame_buffer + _size, &type, sizeof(type)); _size += sizeof(type);
	memcpy(frame_buffer + _size, &msg_id, sizeof(msg_id)); _size += sizeof(msg_id);
	if (size > 0){
		memcpy(frame_buffer + _size, frame, size); _size += size;
	}
	while (_size & 0xF)
		frame_buffer[_size++] = 0;
	frame_buffer[_size - 1] = 0; //crc8(0xFF, frame_buffer, _size - 1);
	if (spp->encryption_enabled){
		//encript
	}
	return spp->send(spp, socket, frame, size);
}
//-----------------------------------------------------------------------------
bool ssp_send_frame_and_wait_for_ack(t_spp *spp, int socket, uint16_t type, uint8_t *frame, uint16_t size, int32_t response_timeout){
	if (!ssp_send_frame(spp, socket, type, frame, size))
		return false;
/*	while (1){
		if (sim7000_tcp_available(t_sim7000 *sim7000, int number, size_t *available)){
			sim7000_tcp_receive(t_sim7000 *sim7000, int number, uint8_t *frame, size_t size){
		}
	}*/
	return true;
}
//-----------------------------------------------------------------------------
bool ssp_send_ack(t_spp *spp, int socket){
	return ssp_send_frame(spp, socket, 0, NULL, 0);
}
//-----------------------------------------------------------------------------
void spp_poll(t_spp *spp){
/*	if (spp->available())
		if (sim7000_tcp_receive(t_sim7000 *sim7000, int number, uint8_t *frame, size_t size)){
			spp_rx()
		}
	}*/
}
//-----------------------------------------------------------------------------
void ssp_init(t_spp *spp, t_receive receive, t_send send, t_available available){
	spp->receive = receive;
	spp->send = send;
	spp->available = available;
	spp->encryption_enabled = false;
}
//-----------------------------------------------------------------------------
