/*
 * spp.h
 *
 * Created: 21/6/2020 15:18
 *  Author: Luis Pichio | luispichio@sistemasindustriales.com | luispichio@gmail.com
 */ 

#ifndef SSP_H_
#define SSP_H_

//-----------------------------------------------------------------------------
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
//-----------------------------------------------------------------------------
typedef bool (*t_receive)(void *, int, uint8_t *, size_t);
typedef bool (*t_send)(void *, int, uint8_t *, size_t);
typedef int (*t_available)(void *, int);
//-----------------------------------------------------------------------------
typedef struct t_spp{
	t_receive receive;
	t_send send;
	t_available available;
	bool encryption_enabled;
} t_spp;
//-----------------------------------------------------------------------------
extern bool ssp_send_frame(t_spp *spp, int socket, uint16_t type, uint8_t *frame, uint16_t size);
extern bool ssp_send_frame_and_wait_for_ack(t_spp *spp, int socket, uint16_t type, uint8_t *frame, uint16_t size, int32_t response_timeout);
extern bool ssp_send_ack(t_spp *spp, int socket);
extern void ssp_init(t_spp *spp, t_receive receive, t_send send, t_available available);
//-----------------------------------------------------------------------------

#endif /* SSP_H_ */
