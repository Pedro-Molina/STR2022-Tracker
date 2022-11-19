/*
 * sim800.h
 *
 * Created: 12/06/2020 23:47 p.m.
 *  Author: Luis Pichio
 */

#ifndef SIM800_H_
#define SIM800_H_

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
	int run_status;
	int fix_status;
	time_t time;
	double latitude;
	double longitude;
	float altitude;
	float speed;
	float course;
	int fix_mode;
	float hdop;
	float pdop;
	float vdop;
	int satellites_in_view;
	int satellites_used;
	float hpa;
	float vpa;
} t_sim800_gnss_report;

typedef enum {
	SIM800_SIM_STATUS_ERROR,
	SIM800_SIM_STATUS_READY,
	SIM800_SIM_STATUS_LOCKED,
	SIM800_SIM_STATUS_ANTITHEFT_LOCKED,
	SIM800_SIM_STATUS_UNKNOWN,
} t_sim800_sim_status;

typedef enum {
	SIM800_REGISTRATION_STATUS_NOT_REGISTERED,
	SIM800_REGISTRATION_STATUS_REGISTERED_HOME,
	SIM800_REGISTRATION_STATUS_SEARCHING,
	SIM800_REGISTRATION_STATUS_DENIED,
	SIM800_REGISTRATION_STATUS_UNKNOWN,
	SIM800_REGISTRATION_STATUS_REGISTERED_ROAMING,
} t_sim800_registration_status;

typedef enum {
	SIM800_PREFERRED_MODE_AUTOMATIC,
	SIM800_PREFERRED_MODE_GSM_ONLY,
	SIM800_PREFERRED_MODE_LTE_ONLY,
	SIM800_PREFERRED_MODE_GSM_AND_LTE_ONLY,
	SIM800_PREFERRED_MODE_UNKNOWN
} t_sim800_preferred_mode;

typedef struct{
	t_sim_core core;
	t_sim800_gnss_report last_gnss_report;
	t_sim800_gnss_report last_gnss_fix_report;
} t_sim800;

extern bool sim800_setup(t_sim800 *sim800);
extern bool sim800_valid_imei_imsi(char *imei_imsi);
extern bool sim800_get_imei(t_sim800 *sim800, char *imei);
extern bool sim800_get_imsi(t_sim800 *sim800, char *imsi);
extern bool sim800_get_rssi_ber(t_sim800 *sim800, int16_t *rssi, int16_t *ber);
extern int16_t sim800_get_rssi(t_sim800 *sim800);
extern t_sim800_sim_status sim800_get_sim_status(t_sim800 *sim800);
extern t_sim800_registration_status sim800_get_network_registration_status(t_sim800 *sim800);
extern t_sim800_preferred_mode sim800_get_preferred_mode(t_sim800 *sim800);
//gprs
extern bool sim800_get_network_apn(t_sim800 *sim800, char *apn);
extern bool sim800_is_gprs_connected(t_sim800 *sim800);
extern bool sim800_get_local_ip(t_sim800 *sim800, char *ip);
extern bool sim800_gprs_connect(t_sim800 *sim800, char *apn, char *user, char *password);
extern bool sim800_gprs_disconnect(t_sim800 *sim800);
//mqtt
extern bool sim800_mqtt_config(t_sim800 *sim800, char* client_id, char* host, int port, char* user_name, char* password, int keep_time, int mqtt_response_timeout);
extern bool sim800_mqtt_connect(t_sim800 *sim800, char *apn, uint32_t response_timeout);
extern bool sim800_mqtt_disconnect(t_sim800 *sim800);
extern bool sim800_mqtt_connected(t_sim800 *sim800);
extern bool sim800_mqtt_pub(t_sim800 *sim800, char *topic, char* content, int qos, int retain);
extern bool sim800_mqtt_sub(t_sim800 *sim800, char* topic, int qos);
extern bool sim800_mqtt_unsub(t_sim800 *sim800, char* topic);
//http
extern bool sim800_http_get(t_sim800 *sim800, char *url, char *response, uint16_t max_response_size);
extern bool sim800_http_init(t_sim800 *sim800);
extern bool sim800_http_terminate(t_sim800 *sim800);
//
extern void sim800_poll(t_sim800 *sim800);
extern void sim800_init(t_sim800 *sim800, uart_port_t uart_num, int32_t frame_timeout);

#endif /* SIM800_H_ */
