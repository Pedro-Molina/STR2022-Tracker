/*
 * sim7000.h
 *
 * Created: 12/06/2020 23:47 p.m.
 *  Author: Luis Pichio
 */

#ifndef SIM7000_H_
#define SIM7000_H_

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include "esp_log.h"
#include "sim_core.h"
#include "simple_timer.h"
#include "utils.h"

typedef struct{
	uint8_t run_status;
	uint8_t fix_status;
	time_t time;
	double latitude;
	double longitude;
	float altitude;
	float speed;
	float course;
	uint8_t fix_mode;
	float hdop;
	float pdop;
	float vdop;
	uint8_t satellites_in_view;
	uint8_t satellites_used;
	float hpa;
	float vpa;
} t_sim7000_gnss_report;

typedef enum {
	SIM_STATUS_ERROR,
	SIM_STATUS_READY,
	SIM_STATUS_LOCKED,
	SIM_STATUS_ANTITHEFT_LOCKED,
	SIM_STATUS_UNKNOWN,
} t_sim_status;

typedef enum {
	REGISTRATION_STATUS_NOT_REGISTERED,
	REGISTRATION_STATUS_REGISTERED_HOME,
	REGISTRATION_STATUS_SEARCHING,
	REGISTRATION_STATUS_DENIED,
	REGISTRATION_STATUS_UNKNOWN,
	REGISTRATION_STATUS_REGISTERED_ROAMING,
} t_registration_status;

typedef enum {
	PREFERRED_MODE_AUTOMATIC,
	PREFERRED_MODE_GSM_ONLY,
	PREFERRED_MODE_LTE_ONLY,
	PREFERRED_MODE_GSM_AND_LTE_ONLY,
	PREFERRED_MODE_UNKNOWN
} t_preferred_mode;

typedef struct{
	t_sim_core core;
	t_sim7000_gnss_report last_gnss_report;
	t_sim7000_gnss_report last_gnss_fix_report;
} t_sim7000;

extern bool sim7000_setup(t_sim7000 *sim7000);
extern bool sim7000_valid_imei_imsi(char *imei_imsi);
extern bool sim7000_get_imei(t_sim7000 *sim7000, char *imei);
extern bool sim7000_get_imsi(t_sim7000 *sim7000, char *imsi);
extern bool sim7000_get_rssi_ber(t_sim7000 *sim7000, int16_t *rssi, int16_t *ber);
extern int16_t sim7000_get_rssi(t_sim7000 *sim7000);
extern t_sim_status sim7000_get_sim_status(t_sim7000 *sim7000);
extern t_registration_status sim7000_get_network_registration_status(t_sim7000 *sim7000);
extern t_preferred_mode sim7000_get_preferred_mode(t_sim7000 *sim7000);
//gprs
extern bool sim7000_get_network_apn(t_sim7000 *sim7000, char *apn);
extern bool sim7000_is_gprs_connected(t_sim7000 *sim7000);
extern bool sim7000_get_local_ip(t_sim7000 *sim7000, char *ip);
extern bool sim7000_gprs_connect(t_sim7000 *sim7000, char *apn, char *user, char *password);
extern bool sim7000_gprs_disconnect(t_sim7000 *sim7000);
//mqtt
extern bool sim7000_mqtt_config(t_sim7000 *sim7000, char* client_id, char* host, int port, char* user_name, char* password, int keep_time);
extern bool sim7000_mqtt_connect(t_sim7000 *sim7000, char *apn, uint32_t response_timeout);
extern bool sim7000_mqtt_disconnect(t_sim7000 *sim7000);
extern bool sim7000_mqtt_connected(t_sim7000 *sim7000);
extern bool sim7000_mqtt_pub(t_sim7000 *sim7000, char *topic, char* content, int qos, int retain);
extern bool sim7000_mqtt_sub(t_sim7000 *sim7000, char* topic, int qos);
extern bool sim7000_mqtt_unsub(t_sim7000 *sim7000, char* topic);
//http
extern bool sim7000_http_get(t_sim7000 *sim7000, char *url, char *response, uint16_t max_response_size);
extern bool sim7000_http_init(t_sim7000 *sim7000);
extern bool sim7000_http_terminate(t_sim7000 *sim7000);
//
extern bool sim7000_tcp_connect(t_sim7000 *sim7000, int number, char *host, int port);
extern bool sim7000_tcp_connected(t_sim7000 *sim7000, int number);
extern bool sim7000_tcp_disconnect(t_sim7000 *sim7000, int number);
extern bool sim7000_tcp_send(t_sim7000 *sim7000, int number, char *content);
extern bool sim7000_tcp_send_frame(t_sim7000 *sim7000, int number, char *frame, size_t size);
extern bool sim7000_tcp_available(t_sim7000 *sim7000, int number, size_t *available);
extern bool sim7000_tcp_receive(t_sim7000 *sim7000, int number, uint8_t *frame, size_t size);
//
extern void sim7000_poll(t_sim7000 *sim7000);
extern void sim7000_init(t_sim7000 *sim7000, uart_port_t uart_num, int32_t frame_timeout);

#endif /* SIM7000_H_ */
