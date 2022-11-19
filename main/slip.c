/*
 * slip.c
 *
 * Created: 24/11/2014 12:14:19 a.m.
 *  Author: Luis Pichio | luispichio@sistemasindustriales.com | luispichio@gmail.com
 */ 
/*
#include "..\includes.h"
//-----------------------------------------------------------------------------
#define SLIP_END						0xC0
#define SLIP_ESC						0xDB
#define SLIP_TEND						0xDC
#define SLIP_TESC						0xDD
//-----------------------------------------------------------------------------
void slip_encode_begin(t_slip *slip){
	slip->buffer_encode_size = 0;
	slip->buffer_encode[slip->buffer_encode_size++] = SLIP_END;
}
//-----------------------------------------------------------------------------
void slip_encode_byte(t_slip *slip, uint8_t byte){
	switch (byte){
		case SLIP_END:
			slip->buffer_encode[slip->buffer_encode_size++] = SLIP_ESC;
			slip->buffer_encode[slip->buffer_encode_size++] = SLIP_TEND;
		break;
		case SLIP_ESC:
			slip->buffer_encode[slip->buffer_encode_size++] = SLIP_ESC;
			slip->buffer_encode[slip->buffer_encode_size++] = SLIP_TESC;
		break;
		default:
			slip->buffer_encode[slip->buffer_encode_size++] = byte;
	}
}
//-----------------------------------------------------------------------------
size_t slip_encode(t_slip *slip, uint8_t *source_frame, uint16_t source_frame_size, uint8_t *encoded_frame){
	size_t encoded_frame_size = 0;
	while (source_frame_size){
		switch (*source_frame){
			case SLIP_END:
				encoded_frame[encoded_frame_size++] = SLIP_ESC;
				encoded_frame[encoded_frame_size++] = SLIP_TEND;
			break;
			case SLIP_ESC:
				encoded_frame[encoded_frame_size++] = SLIP_ESC;
				encoded_frame[encoded_frame_size++] = SLIP_TESC;
			break;
			default:
				encoded_frame[encoded_frame_size++] = *source_frame;
		}
		source_frame_size--;
		source_frame++;
	}
	return encoded_frame_size;
}
//-----------------------------------------------------------------------------
size_t slip_decode(t_slip *slip, uint8_t *encoded_frame, uint16_t encoded_frame_size, uint8_t *decoded_frame){
	uint8_t byte;
	size_t result = 0;
	while (encoded_frame_size){
		byte = *encoded_frame;
		switch (byte){
			case SLIP_END:
				slip->esc_received = false;
				if (slip->decoded_frame_size){
					memcpy(decoded_frame, slip->decoded_frame);
					result = slip->buffer_decode_size;
					slip->buffer_decode_size = 0;
				}
			break;
			case SLIP_ESC:
				slip->esc_received = true;
			break;
			default:
				if (slip->esc_received){
					switch (byte){
						case SLIP_TEND:
							byte = SLIP_END;
						break;
						case SLIP_TESC:
							byte = SLIP_ESC;
						break;
					}
					slip->esc_received = false;
				}
				if (slip->decoded_frame_size < SLIP_BUFFER_DECODE_SIZE)
					slip->decoded_frame[slip->decoded_frame_size++] = byte;
		}
		encoded_frame++;
		encoded_frame_size--;
	}
	return result;
}
//-----------------------------------------------------------------------------
void slip_decode_byte(t_slip *slip, uint8_t byte){
	switch (byte){
		case SLIP_END:
			slip->esc_received = false;
			if (slip->buffer_decode_size){
				if (slip->on_slip_frame_decoded)
					slip->on_slip_frame_decoded(slip, slip->buffer_decode, slip->buffer_decode_size);
				slip->buffer_decode_size = 0;
			}
		break;
		case SLIP_ESC:
			slip->esc_received = true;
		break;
		default:
			if (slip->esc_received){
				switch (byte){
					case SLIP_TEND:
						byte = SLIP_END;
					break;
					case SLIP_TESC:
						byte = SLIP_ESC;
					break;
				}
				slip->esc_received = false;
			}
			if (slip->buffer_decode_size < SLIP_BUFFER_DECODE_SIZE)
				slip->buffer_decode[slip->buffer_decode_size++] = byte;
	}
}
//-----------------------------------------------------------------------------
void slip_init(t_slip *slip, t_slip_frame_decoded on_slip_frame_decoded){
	slip->on_slip_frame_decoded = on_slip_frame_decoded;
	slip->buffer_decode_size = 0;
	slip->buffer_encode_size = 0;
	slip->esc_received = 0;
}
//-----------------------------------------------------------------------------
*/
