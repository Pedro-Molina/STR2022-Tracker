/*
 * sim_core.h
 *
 * Created: 12/06/2020 18:35 p.m.
 *  Author: Luis Pichio
 */ 
#ifndef SIM_CORE_H_
#define SIM_CORE_H_

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "driver/uart.h"
#include "esp_log.h"
#include "simple_timer.h"

#define CRLF              							"\r\n"

#define SIM_CORE_POLL_LINE_SIZE					1024
#define SIM_CORE_RESPONSE_SIZE					1024
#define SIM_CORE_URC_SIZE						512
#define SIM_CORE_ASYNC_CONTENT_SIZE				512

typedef enum {
	ERROR_TYPE_COMMON,
	ERROR_TYPE_CMS,
	ERROR_TYPE_CME,
} t_error_type;

typedef struct{
	uart_port_t uart_num;
	const char **urcs;

	char line[SIM_CORE_POLL_LINE_SIZE];
	int line_length;
	//
	char response[SIM_CORE_RESPONSE_SIZE];
	char urc[SIM_CORE_URC_SIZE];
	char async_content[SIM_CORE_ASYNC_CONTENT_SIZE];

	bool wait_for_prompt;
	//flags
	bool flag_response_received;
	bool flag_urc_received;
	bool flag_ok_received;
	bool flag_atc_transaction;
	bool flag_async_content_received;
	bool flag_error_received;
	bool flag_prompt_received;

	t_error_type error_type;
	int error_code;

	//timers
	t_simple_timer timer_response_timeout;
	t_simple_timer timer_frame_timeout;
	SemaphoreHandle_t xSemaphore;
} t_sim_core;

extern int8_t sim_core_rssi_to_dbm(uint8_t rssi);
extern void sim_core_flush_rx(t_sim_core *sim_core);
extern void sim_core_flush_response(t_sim_core *sim_core);
extern void sim_core_flush_urc(t_sim_core *sim_core);
extern void sim_core_flush_async_content(t_sim_core *sim_core);
extern void wait_for_prompt(t_sim_core *sim_core);
extern size_t sim_core_available_for_rx(t_sim_core *sim_core);
extern bool sim_core_receive_frame(t_sim_core *sim_core, uint8_t *frame, size_t size);
extern bool sim_core_send_frame(t_sim_core *sim_core, uint8_t *frame, size_t size);
extern bool sim_core_send(t_sim_core *sim_core, char *s);
extern bool sim_core_send_at(t_sim_core *sim_core, char *at, int32_t response_timeout);
extern bool sim_core_waiting_for_response(t_sim_core *sim_core);
extern bool sim_core_response_timeout(t_sim_core *sim_core);
extern bool sim_core_response_contains(char *response, char *content);
extern void sim_core_poll(t_sim_core *sim_core);
extern bool sim_core_response_received(t_sim_core *sim_core, char *response, bool *ok, bool *error);
extern bool sim_core_urc_received(t_sim_core *sim_core, char *urc);
extern bool sim_core_async_content_received(t_sim_core *sim_core, char *content);
extern bool sim_core_prompt_received(t_sim_core *sim_core);
extern bool sim_core_wait_for_response(t_sim_core *sim_core, char *response, bool *ok, bool *error, bool *timeout);
extern bool sim_core_wait_for_urc(t_sim_core *sim_core, char *urc, uint32_t response_timeout, bool *timeout);
extern bool sim_core_wait_for_async_content(t_sim_core *sim_core, char *content, uint32_t response_timeout, bool *timeout);
extern bool sim_core_send_at_and_wait_for_response(t_sim_core *sim_core, const char *at, char *response, uint32_t response_timeout, bool *ok, bool *error, bool *timeou);
extern bool sim_core_send_and_wait_for_prompt(t_sim_core *sim_core, const char *content, uint32_t response_timeout, bool *timeout);
extern void sim_core_init(t_sim_core *sim_core, uart_port_t uart_num, const char **urcs, int32_t frame_timeout);

#endif /* SIM_CORE_H_ */
