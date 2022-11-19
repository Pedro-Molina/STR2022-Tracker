/*
 * sim_core.c
 *
 * Created: 12/06/2020 18:35 p.m.
 *  Author: Luis Pichio
 */
#include "sim_core.h"

#define DEBUG_SIM_CORE
const char* SIM_CORE_TAG = "SIM_CORE";

//-----------------------------------------------------------------------------
int _sim_core_urc_index(t_sim_core *sim_core, char *s);
bool _sim_core_is_urc(t_sim_core *sim_core, char *s);
bool _sim_core_send(t_sim_core *sim_core, char *s);
void _sim_core_process_line(t_sim_core *sim_core, char *line);
void _sim700_core_process_frame(t_sim_core *sim_core, uint8_t *frame, size_t frame_size);
//-----------------------------------------------------------------------------
int8_t sim_core_rssi_to_dbm(uint8_t rssi){
	if (rssi == 0)
		return -115;
	if (rssi == 1)
		return -111;
	if (rssi == 99)
		return 0;
	if ((rssi >= 2) && (rssi <= 31))
		return -110 + (rssi - 2) * 2;
	return 0;
}
//-----------------------------------------------------------------------------
void sim_core_flush_rx(t_sim_core *sim_core){
	if (xSemaphoreTake(sim_core->xSemaphore, portMAX_DELAY) == pdTRUE){  //sync
		uart_flush(sim_core->uart_num);
		strcpy(sim_core->line, "");
		sim_core->line_length = 0;
		xSemaphoreGive(sim_core->xSemaphore);
	}
}
//-----------------------------------------------------------------------------
void _sim_core_flush_response(t_sim_core *sim_core){
	strcpy(sim_core->response, "");
	sim_core->flag_response_received = false;
	sim_core->flag_ok_received = false;
	sim_core->flag_error_received = false;
	sim_core->flag_async_content_received = false;
}
//-----------------------------------------------------------------------------
void _sim_core_flush_urc(t_sim_core *sim_core){
	strcpy(sim_core->urc, "");
	sim_core->flag_urc_received = false;
}
//-----------------------------------------------------------------------------
void _sim_core_flush_async_content(t_sim_core *sim_core){
	strcpy(sim_core->async_content, "");
	sim_core->flag_async_content_received = false;
}
//-----------------------------------------------------------------------------
void sim_core_flush_response(t_sim_core *sim_core){
	if (xSemaphoreTake(sim_core->xSemaphore, portMAX_DELAY) == pdTRUE){  //sync
		strcpy(sim_core->response, "");
		sim_core->flag_response_received = false;
		sim_core->flag_ok_received = false;
		sim_core->flag_error_received = false;
		xSemaphoreGive(sim_core->xSemaphore);
	}
}
//-----------------------------------------------------------------------------
void sim_core_flush_urc(t_sim_core *sim_core){
	if (xSemaphoreTake(sim_core->xSemaphore, portMAX_DELAY) == pdTRUE){  //sync
		strcpy(sim_core->urc, "");
		sim_core->flag_urc_received = false;
		xSemaphoreGive(sim_core->xSemaphore);
	}
}
//-----------------------------------------------------------------------------
void sim_core_flush_async_content(t_sim_core *sim_core){
	if (xSemaphoreTake(sim_core->xSemaphore, portMAX_DELAY) == pdTRUE){  //sync
		strcpy(sim_core->async_content, "");
			sim_core->flag_async_content_received = false;
		xSemaphoreGive(sim_core->xSemaphore);
	}
}
//-----------------------------------------------------------------------------
void wait_for_prompt(t_sim_core *sim_core){
	if (xSemaphoreTake(sim_core->xSemaphore, portMAX_DELAY) == pdTRUE){  //sync
		sim_core->flag_prompt_received = false;
		sim_core->wait_for_prompt = true;
		xSemaphoreGive(sim_core->xSemaphore);
	}
}
//-----------------------------------------------------------------------------
size_t sim_core_available_for_rx(t_sim_core *sim_core){
	size_t available = 0;
	uart_get_buffered_data_len(sim_core->uart_num, &available);
	return available;
}
//-----------------------------------------------------------------------------
bool sim_core_receive_frame(t_sim_core *sim_core, uint8_t *frame, size_t size){
	return uart_read_bytes(sim_core->uart_num, frame, size, 0) == size;
}
//-----------------------------------------------------------------------------
bool _sim_core_send_frame(t_sim_core *sim_core, uint8_t *frame, size_t size){
#ifdef DEBUG_SIM_CORE_LOW_LEVEL
	ESP_LOGI(SIM_CORE_TAG, "sim_core_send_frame(%d)", size);
	ESP_LOG_BUFFER_HEXDUMP(SIM_CORE_TAG, frame, size, ESP_LOG_INFO);
#endif
	return uart_write_bytes(sim_core->uart_num, (char *)frame, size) == size;
}
//-----------------------------------------------------------------------------
bool sim_core_send_frame(t_sim_core *sim_core, uint8_t *frame, size_t size){
	bool result = false;
	if (xSemaphoreTake(sim_core->xSemaphore, portMAX_DELAY) == pdTRUE){  //sync
		result = _sim_core_send_frame(sim_core, frame, size);
		xSemaphoreGive(sim_core->xSemaphore);
	}
	return result;
}
//-----------------------------------------------------------------------------
bool _sim_core_send(t_sim_core *sim_core, char *s){
	return _sim_core_send_frame(sim_core, (uint8_t *)s, strlen(s));
}
//-----------------------------------------------------------------------------
bool sim_core_send(t_sim_core *sim_core, char *s){
	bool result = false;
	if (xSemaphoreTake(sim_core->xSemaphore, portMAX_DELAY) == pdTRUE){  //sync
		result = _sim_core_send(sim_core, s);
		xSemaphoreGive(sim_core->xSemaphore);
	}
	return result;
}
//-----------------------------------------------------------------------------
bool sim_core_send_at(t_sim_core *sim_core, char *at, int32_t response_timeout){
	bool result = false;
#ifdef DEBUG_SIM_CORE
	ESP_LOGI(SIM_CORE_TAG, "sim_core_send_at() -> %s", at);
#endif
	if (xSemaphoreTake(sim_core->xSemaphore, portMAX_DELAY) == pdTRUE){  //sync
		if ((result = _sim_core_send(sim_core, at))){
			simple_timer_start(&sim_core->timer_response_timeout, response_timeout);
			sim_core->flag_atc_transaction = true;
		}
		xSemaphoreGive(sim_core->xSemaphore);
	}
	return result;
}
//-----------------------------------------------------------------------------
bool sim_core_waiting_for_response(t_sim_core *sim_core){
	return sim_core->flag_atc_transaction;
}
//-----------------------------------------------------------------------------
bool sim_core_response_timeout(t_sim_core *sim_core){
	return (sim_core->flag_atc_transaction && simple_timer_timeout(&sim_core->timer_response_timeout));
}
//-----------------------------------------------------------------------------
int _sim_core_urc_index(t_sim_core *sim_core, char *s){
	int i = 0;
	while (sim_core->urcs[i] != NULL && (strstr(s, sim_core->urcs[i]) != s))				//buscamos en la lista de urc's
		i++;
	return (sim_core->urcs[i] != NULL) ? i : -1;
}
//-----------------------------------------------------------------------------
bool _sim_core_is_urc(t_sim_core *sim_core, char *s){
	return _sim_core_urc_index(sim_core, s) != -1;
}
//-----------------------------------------------------------------------------
bool sim_core_response_contains(char *response, char *content){
	return strstr(response, content) != NULL;
}
//-----------------------------------------------------------------------------
void _sim_core_process_line(t_sim_core *sim_core, char *line){
	if (strlen(line) < 2)
		return;
#ifdef DEBUG_SIM_CORE
	ESP_LOGI(SIM_CORE_TAG, "_sim_core_process_line() -> %s", line);
#endif
	if (_sim_core_is_urc(sim_core, line)){														//si la línea es URC
		if (strlen(sim_core->urc) + strlen(line) + 2 < SIM_CORE_URC_SIZE){		//si hay espacio en buffer
			strcat(sim_core->urc, line);											//la agregamos al buffer de urc's
			strcat(sim_core->urc, CRLF);
		}
		sim_core->flag_urc_received = true;
		return;
	}
	if (sim_core->flag_atc_transaction){
		sim_core->flag_response_received = true;
		if (strlen(line) == 2 && strstr(line, "OK")){
			sim_core->flag_ok_received = true;
			return;
		}
		if (strstr(line, "ERROR") == line){
			sim_core->flag_error_received = true;
			sim_core->error_type = ERROR_TYPE_COMMON;
			return;
		}
		if (strstr(line, "+CME ERROR:") == line){
			sim_core->flag_error_received = true;
			sim_core->error_type = ERROR_TYPE_CME;
			sim_core->error_code = 0;
			return;
		}

		if (strstr(line, "+CMS ERROR:") == line){
			sim_core->flag_error_received = true;
			sim_core->error_type = ERROR_TYPE_CMS;
			sim_core->error_code = 0;
			return;
		}
		if (strlen(sim_core->response) + strlen(line) + 2 < SIM_CORE_RESPONSE_SIZE){	//si hay espacio en buffer
			strcat(sim_core->response, line);											//la agregamos al buffer de respuestas
			strcat(sim_core->response, CRLF);
		}
	} else {
		if (strlen(sim_core->async_content) + strlen(line) + 2 < SIM_CORE_ASYNC_CONTENT_SIZE){	//si hay espacio en buffer
			sim_core->flag_async_content_received = true;
			strcat(sim_core->async_content, line);												//la agregamos al buffer de respuestas
			strcat(sim_core->async_content, CRLF);
		}
	}
}
//-----------------------------------------------------------------------------
void _sim700_core_process_frame(t_sim_core *sim_core, uint8_t *frame, size_t frame_size){
#ifdef DEBUG_SIM_CORE_LOW_LEVEL
	ESP_LOGI(SIM_CORE_TAG, "_sim700_core_process_frame(%d)", frame_size);
	ESP_LOG_BUFFER_HEXDUMP(SIM_CORE_TAG, frame, frame_size, ESP_LOG_INFO);
#endif
	for (size_t i = 0 ; i < frame_size ; i++){
		switch (frame[i]){
			case '\r':
			break;
			case '\n':
				if (sim_core->line_length){
					sim_core->line[sim_core->line_length] = 0;
					_sim_core_process_line(sim_core, sim_core->line);
					sim_core->line_length = 0;
				}
			break;
			case '>':
				if (sim_core->wait_for_prompt){
					sim_core->wait_for_prompt = false;
					sim_core->flag_prompt_received = true;
					break;
				}
			default:
				sim_core->line[sim_core->line_length++] = frame[i];
		}
	}
}
//-----------------------------------------------------------------------------
#define SIM_CORE_POLL_BUFFER_SIZE			128
#define MIN(A, B) ((A) < (B) ? (A) : (B))
void sim_core_poll(t_sim_core *sim_core){
	uint8_t frame[SIM_CORE_POLL_BUFFER_SIZE];
	size_t available_for_rx = sim_core_available_for_rx(sim_core);
	if (available_for_rx && xSemaphoreTake(sim_core->xSemaphore, 0) == pdTRUE){  //sync
		simple_timer_reset(&sim_core->timer_frame_timeout);
		do {
			if (available_for_rx){
				size_t frame_size = MIN(available_for_rx, SIM_CORE_POLL_BUFFER_SIZE);
				if (sim_core_receive_frame(sim_core, frame, frame_size))
					_sim700_core_process_frame(sim_core, frame, frame_size);
			} else
				vTaskDelay(10 / portTICK_PERIOD_MS);
			if ((available_for_rx = sim_core_available_for_rx(sim_core)))
				simple_timer_reset(&sim_core->timer_frame_timeout);
		} while (!simple_timer_timeout(&sim_core->timer_frame_timeout));  //mientras no haya timeout de frame

		if (sim_core->flag_response_received)
			sim_core->flag_atc_transaction = false;
		xSemaphoreGive(sim_core->xSemaphore);
	}
}
//-----------------------------------------------------------------------------
bool sim_core_response_received(t_sim_core *sim_core, char *response, bool *ok, bool *error){
	bool result = false;
	if (xSemaphoreTake(sim_core->xSemaphore, portMAX_DELAY) == pdTRUE){  //sync
		if (sim_core->flag_response_received){
			if (response) strcpy(response, sim_core->response);
			if (ok) *ok = sim_core->flag_ok_received;
			if (error) *error = sim_core->flag_error_received;
			_sim_core_flush_response(sim_core);
			result = true;
		}
		xSemaphoreGive(sim_core->xSemaphore);
	}
	return result;
}
//-----------------------------------------------------------------------------
bool sim_core_urc_received(t_sim_core *sim_core, char *urc){
	bool result = false;
	if (xSemaphoreTake(sim_core->xSemaphore, portMAX_DELAY) == pdTRUE){  //sync
		if (sim_core->flag_urc_received){
			if (urc)
				strcpy(urc, sim_core->urc);
			_sim_core_flush_urc(sim_core);
			result = true;
		}
		xSemaphoreGive(sim_core->xSemaphore);
	}
	return result;
}
//-----------------------------------------------------------------------------
bool sim_core_async_content_received(t_sim_core *sim_core, char *content){
	bool result = false;
	if (xSemaphoreTake(sim_core->xSemaphore, portMAX_DELAY) == pdTRUE){  //sync
		if (sim_core->flag_async_content_received){
			if (content)
				strcpy(content, sim_core->async_content);
			_sim_core_flush_async_content(sim_core);
			result = true;
		}
		xSemaphoreGive(sim_core->xSemaphore);
	}
	return result;
}
//-----------------------------------------------------------------------------
bool sim_core_prompt_received(t_sim_core *sim_core){
	bool result = false;
	if (xSemaphoreTake(sim_core->xSemaphore, portMAX_DELAY) == pdTRUE){  //sync
		if (sim_core->flag_prompt_received){
			sim_core->flag_prompt_received = false;
			result = true;
		}
		xSemaphoreGive(sim_core->xSemaphore);
	}
	return result;
}
//-----------------------------------------------------------------------------
bool sim_core_wait_for_response(t_sim_core *sim_core, char *response, bool *ok, bool *error, bool *timeout){
	*timeout = false;
	while (1){
		if (sim_core_response_received(sim_core, response, ok, error))
			return true;
		if (sim_core_response_timeout(sim_core)){
			*timeout = true;
			return false;
		}
		vTaskDelay(10 / portTICK_PERIOD_MS);
	}
}
//-----------------------------------------------------------------------------
bool sim_core_wait_for_urc(t_sim_core *sim_core, char *urc, uint32_t response_timeout, bool *timeout){
	t_simple_timer timer_timeout;
	*timeout = false;
	simple_timer_start(&timer_timeout, response_timeout);
	while (!simple_timer_timeout(&timer_timeout)){
		if (sim_core_urc_received(sim_core, urc))
			return true;
		vTaskDelay(10 / portTICK_PERIOD_MS);
	}
	*timeout = true;
	return false;
}
//-----------------------------------------------------------------------------
bool sim_core_wait_for_async_content(t_sim_core *sim_core, char *content, uint32_t response_timeout, bool *timeout){
	t_simple_timer timer_timeout;
	*timeout = false;
	simple_timer_start(&timer_timeout, response_timeout);
	while (!simple_timer_timeout(&timer_timeout)){
		if (sim_core_async_content_received(sim_core, content))
			return true;
		vTaskDelay(10 / portTICK_PERIOD_MS);
	}
	*timeout = true;
	return false;
}
//-----------------------------------------------------------------------------
bool sim_core_wait_for_prompt(t_sim_core *sim_core, uint32_t response_timeout, bool *timeout){
	t_simple_timer timer_timeout;
	*timeout = false;
	simple_timer_start(&timer_timeout, response_timeout);
	while (!simple_timer_timeout(&timer_timeout)){
		if (sim_core_prompt_received(sim_core))
			return true;
		vTaskDelay(10 / portTICK_PERIOD_MS);
	}
	*timeout = true;
	return false;
}
//-----------------------------------------------------------------------------
bool sim_core_send_at_and_wait_for_response(t_sim_core *sim_core, const char *at, char *response, uint32_t response_timeout, bool *ok, bool *error, bool *timeout){
	sim_core_flush_response(sim_core);
	if (!sim_core_send_at(sim_core, at, response_timeout))
		return false;
	return sim_core_wait_for_response(sim_core, response, ok, error, timeout);
}
//-----------------------------------------------------------------------------
bool sim_core_send_and_wait_for_prompt(t_sim_core *sim_core, const char *content, uint32_t response_timeout, bool *timeout){
	wait_for_prompt(sim_core);
	if (!sim_core_send(sim_core, content))
		return false;
	return sim_core_wait_for_prompt(sim_core, response_timeout, timeout);
}
//-----------------------------------------------------------------------------
void sim_core_init(t_sim_core *sim_core, uart_port_t uart_num, const char **urcs, int32_t frame_timeout){
#ifdef DEBUG_SIM_CORE
	esp_log_level_set(SIM_CORE_TAG, ESP_LOG_INFO);
	ESP_LOGI(SIM_CORE_TAG, "sim_core_init()");
#endif
	sim_core->uart_num = uart_num;
	sim_core->urcs = urcs;
	simple_timer_start(&sim_core->timer_frame_timeout, frame_timeout);
	strcpy(sim_core->urc, "");
	strcpy(sim_core->response, "");
	strcpy(sim_core->async_content, "");

	sim_core->wait_for_prompt = false;
	//flags
	sim_core->flag_response_received = false;
	sim_core->flag_urc_received = false;
	sim_core->flag_ok_received = false;
	sim_core->flag_error_received = false;
	sim_core->flag_atc_transaction = false;
	sim_core->flag_async_content_received = false;
	sim_core->flag_error_received = false;
	sim_core->flag_prompt_received = false;

	sim_core->xSemaphore = xSemaphoreCreateBinary();
	xSemaphoreGive(sim_core->xSemaphore);
}
//-----------------------------------------------------------------------------
