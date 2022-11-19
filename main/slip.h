/*
 * slip.h
 *
 * Created: 24/11/2014 12:30:49 a.m.
 *  Author: Luis Pichio | luispichio@sistemasindustriales.com | luispichio@gmail.com
 */ 


#ifndef SLIP_H_
#define SLIP_H_
/*
//-----------------------------------------------------------------------------
#define SLIP_BUFFER_DECODE_SIZE			128
#define SLIP_BUFFER_ENCODE_SIZE			256
//-----------------------------------------------------------------------------
typedef void (*t_slip_frame_decoded)(void *slip, uint8_t *frame, uint16_t size);
//-----------------------------------------------------------------------------
typedef struct t_slip{
	uint8_t buffer_decode[SLIP_BUFFER_DECODE_SIZE];
	uint16_t buffer_decode_size;
	uint8_t buffer_encode[SLIP_BUFFER_ENCODE_SIZE];
	uint16_t buffer_encode_size;
	t_slip_frame_decoded on_slip_frame_decoded;
	bool esc_received;
} t_slip;
//-----------------------------------------------------------------------------
extern void slip_encode_begin(t_slip *slip);
extern void slip_encode_byte(t_slip *slip, uint8_t byte);
extern void slip_encode(t_slip *slip, uint8_t *frame, uint16_t frame_size);
extern uint8_t *slip_encode_end(t_slip *slip, uint16_t *size);
extern void slip_decode(t_slip *slip, uint8_t *frame, uint16_t size);
extern void slip_decode_byte(t_slip *slip, uint8_t byte);
extern void slip_init(t_slip *slip, t_slip_frame_decoded on_slip_frame_decoded);
//-----------------------------------------------------------------------------
*/
#endif /* SLIP_H_ */
