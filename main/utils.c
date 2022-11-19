/*
 * utils.c
 *
 * Created: 25/07/2014 01:02:22 p.m.
 *  Author: Luis Pichio | luispichio@sistemasindustriales.com | luispichio@gmail.com
 */ 
#include "utils.h"

//------------------------------------------------------------------------------
uint64_t utils_hours_to_ms(float hours){
	return (uint64_t)(hours * 3600000.0f);
}
//------------------------------------------------------------------------------
uint64_t utils_minutes_to_ms(float minutes){
	return (uint64_t)(minutes * 60000.0f);
}
//------------------------------------------------------------------------------
uint64_t utils_seconds_to_ms(float seconds){
	return (uint64_t)(seconds * 1000.0f);
}
//------------------------------------------------------------------------------
uint8_t utils_char2bcd(uint8_t data){
	return (data % 10) + 16 * (data / 10);
}
//------------------------------------------------------------------------------
uint8_t utils_bcd2char(uint8_t data){
	return (data & 0x0f) + 10 * (data / 16);
}
//------------------------------------------------------------------------------
uint32_t utils_pow_uint32(uint32_t base, uint8_t exp){
	uint32_t rval = 1;
	while (exp--)
		rval *= base;
	return (rval);
}
//------------------------------------------------------------------------------
void utils_set_digit(unsigned long *val, char ndigit, char digit){
	*val -= ((*val / utils_pow_uint32(10, ndigit)) % 10) * utils_pow_uint32(10, ndigit);
	*val += digit * utils_pow_uint32(10, ndigit);
}
//------------------------------------------------------------------------------
unsigned char utils_get_digit(unsigned long val, char digit){
	return (val / utils_pow_uint32(10, digit)) % 10;
}
//------------------------------------------------------------------------------
void utils_inc_digit(unsigned long *val, unsigned char digit){
	utils_set_digit(val, digit, (utils_get_digit(*val, digit) + 1) % 10);
}
//-----------------------------------------------------------------------------
char *utils_str_trim(char *str){
	uint16_t len;
	while ((len = strlen(str)) && (str[len - 1] == ' '))
		str[len - 1] = 0;
	char *aux = str;
	while (strlen(aux) && (aux[0] == ' '))
		aux++;
	memmove(str, aux, strlen(aux) + 1);
	return str;	
}
//------------------------------------------------------------------------------
char *utils_str_pad(char *str, uint8_t len){
	while (strlen(str) < len)
		strcat(str, " ");
	return str;
}
//------------------------------------------------------------------------------
char *utils_str_char_replace(char *str, char search, char replace){
	for (uint8_t i = 0 ; i < strlen(str) ; i++)
		if (str[i] == search)
			str[i] = replace;
	return str;
}
//------------------------------------------------------------------------------
char *utils_str_char_remove(char *str, char chr){
	char *aux1 = str;
	char *aux2 = str;
	while (*aux1){
		if (*aux1 != chr){
			*aux2 = *aux1;
			aux2++;
		}
		aux1++;
	}
	*aux2 = 0;
	return str;
}
//------------------------------------------------------------------------------
char *utils_to_upper_case(char *str){
	for (uint8_t i = 0 ; i < strlen(str) ; i++)
		str[i] = toupper(str[i]);
	return str;
}
//------------------------------------------------------------------------------
long utils_mapl(long x, long in_min, long in_max, long out_min, long out_max){
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
//------------------------------------------------------------------------------
int utils_mapi(int x, int in_min, int in_max, int out_min, int out_max){
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
//------------------------------------------------------------------------------
char *utils_token(char *s, char delimiter, uint8_t number, char *token, uint16_t maxTokenLen){
	char *_token = token;
	while (*s && number){
		if (*s == delimiter)
			number--;
		s++;
	}
	while (*s && maxTokenLen && (*s != delimiter)){
		*_token = *s;
		s++;
		_token++;
		maxTokenLen--;
	}
	*_token = 0;
	return token;
}
//-----------------------------------------------------------------------------
int32_t utils_tokenl(char *s, char delimiter, uint8_t number){
	char str[16];
	return atol(utils_token(s, delimiter, number, str, 15));
}
//-----------------------------------------------------------------------------
int8_t utils_first_bit(uint64_t mask){
	int8_t result = 0;
	while (mask){
		if (mask & 1)
			return result;
		mask >>= 1;
		result++;
	}
	return -1;
}
//-----------------------------------------------------------------------------
char *utils_frame_to_hex(char *frame, uint16_t size, char *str){
	char aux[3];
	strcpy(str, "");
	while (size--){
		sprintf(aux, "%02X", *frame);
		strcat(str, aux);
		frame++;
	}
	return str;	
}
//------------------------------------------------------------------------------
