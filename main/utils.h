/*
 * utils.h
 *
 * Created: 25/07/2014 01:02:42 p.m.
 *  Author: Luis Pichio | luispichio@sistemasindustriales.com | luispichio@gmail.com
 */ 


#ifndef UTILS_H_
#define UTILS_H_

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

extern uint64_t utils_hours_to_ms(float hours);
extern uint64_t utils_minutes_to_ms(float minutes);
extern uint64_t utils_seconds_to_ms(float seconds);
extern uint8_t utils_char2bcd(uint8_t data);
extern uint8_t utils_bcd2char(uint8_t data);
extern uint32_t utils_pow_uint32(uint32_t base, uint8_t exp);
extern void utils_set_digit(unsigned long *val, char ndigit, char digit);
extern unsigned char utils_get_digit(unsigned long val, char digit);
extern void utils_inc_digit(unsigned long *val, unsigned char digit);
extern char *utils_str_trim(char *str);
extern char *utils_str_pad(char *str, uint8_t len);
extern char *utils_str_char_replace(char *str, char search, char replace);
extern char *utils_str_char_remove(char *str, char chr);
extern char *utils_to_upper_case(char *str);
extern long utils_mapl(long x, long in_min, long in_max, long out_min, long out_max);
extern int utils_mapi(int x, int in_min, int in_max, int out_min, int out_max);
extern char *utils_token(char *s, char delimiter, uint8_t number, char *token, uint16_t maxTokenLen);
extern int32_t utils_tokenl(char *s, char delimiter, uint8_t number);
extern int8_t utils_first_bit(uint64_t mask);
extern char *utils_frame_to_hex(char *frame, uint16_t size, char *str);
	
#endif /* UTILS_H_ */
