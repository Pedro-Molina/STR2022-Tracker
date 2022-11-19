#include "sim_core.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "string.h"
#include "driver/gpio.h"
#include "sim7000.h"
#include "sim800.h"

//#include "MQTTPacket.h"

#include "MQTTFreeRTOS.h"
//#define MQTT_TASK 1
#include "MQTTClient.h"
//-----------------------------------------------------------------------------
typedef struct __attribute__((__packed__)){
	uint16_t esn;
	uint16_t id;
	uint16_t type;
//
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

	uint8_t in_movement;
	uint8_t new_track;
	uint8_t reserved0;
	uint32_t odometer;
} t_gnss_report;
//-----------------------------------------------------------------------------
//#define SIM800
#define SIM7000
//-----------------------------------------------------------------------------
#define MODEM_TXD_PIN (GPIO_NUM_23)
#define MODEM_RXD_PIN (GPIO_NUM_22)
#define MODEM_SLEEP_GPS_ENABLE_PIN (GPIO_NUM_18)
#define MODEM_PWRKEY_PIN (GPIO_NUM_19)
//-----------------------------------------------------------------------------
#define BLINK_GPIO (GPIO_NUM_14)
//t_sim7000_core sim7000_core;
t_sim7000 sim7000;
t_sim800 sim800;
//-----------------------------------------------------------------------------
#define MQTT_CLIENT_ID "client_id"
#define MQTT_HOST "rcceae11.us-east-1.emqx.cloud"
#define MQTT_PORT 15086
#define MQTT_USER "54321"
#define MQTT_PASS "54321"
#define MQTT_TOPIC "COORDENADAS"

Network network;
MQTTClient mqttClient;
unsigned char mqtt_sendbuf[80], mqtt_readbuf[80];
MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;

//-----------------------------------------------------------------------------
const char* MODEM_TASK_TAG = "MODEM_TASK";
const char* REPORT_TASK_TAG = "REPORT_TASK";
const char* MODEM_CORE_TASK_TAG = "MODEM_CORE_TASK";
const char* STATUS_TASK_TAG = "STATUS_TASK";
//-----------------------------------------------------------------------------
/**
 * Completa el buffer con la información del GNSS Report
 * @param report
 * @param buffer Cadena de salida
 */
char* gnss_report_2_string(t_gnss_report *report, char * buffer) {
    sprintf(buffer, "%.6f,%.6f", report->latitude, report->longitude);
    return buffer;
}
/**
 * Transforma de tipos de datos sim7000_gnss_report a gnss_report
 * @param sim7000_report
 * @param report
 * @param esn
 * @param id
 * @param type
 */
//-----------------------------------------------------------------------------

void sim7000_gnss_report_2_gnss_report(t_sim7000_gnss_report *sim7000_report, t_gnss_report *report, uint16_t esn, uint16_t id, uint16_t type){
	report->esn = esn;
	report->id = id;
	report->type = type;

	report->run_status = sim7000_report->run_status;
	report->fix_status = sim7000_report->fix_status;
	report->time = sim7000_report->time;
	report->latitude = sim7000_report->latitude;
	report->longitude = sim7000_report->longitude;
	report->altitude = sim7000_report->altitude;
	report->speed = sim7000_report->speed;
	report->course = sim7000_report->course;
	report->fix_mode = sim7000_report->fix_mode;
	report->hdop = sim7000_report->hdop;
	report->pdop = sim7000_report->pdop;
	report->vdop = sim7000_report->vdop;
	report->satellites_in_view = sim7000_report->satellites_in_view;
	report->satellites_used = sim7000_report->satellites_used;
	report->hpa = sim7000_report->hpa;
	report->vpa	 = sim7000_report->vpa;

	report->in_movement = 0;
	report->new_track = 0;
	report->reserved0 = 0;
	report->odometer = 0;
}
//-----------------------------------------------------------------------------
//const char* C_SERVER_IP = "181.170.73.71";
const char* C_SERVER_IP = "191.85.30.22";
//-----------------------------------------------------------------------------
bool ready4Report(){
#ifdef SIM7000
	if (!sim7000_is_gprs_connected(&sim7000)){
		//if (!sim7000_gprs_connect(&sim7000, "datos.personal.com", "datos","datos"))
		if (!sim7000_gprs_connect(&sim7000, "datos.personal.com", "datos","datos"))
			return false;
	}
	//if (!sim7000_tcp_connected(&sim7000, 0) && !sim7000_tcp_connect(&sim7000, 0, C_SERVER_IP, 1883)){
	//	ESP_LOGI(MODEM_TASK_TAG, "NO SE CONECTO A TCP SALAME");
	//	return false;

	//}else{
	//	ESP_LOGI(MODEM_TASK_TAG, "SE CONECTO A TCP SALAME");

	//}

	if (!sim7000_tcp_connected(&sim7000, 0) && !sim7000_tcp_connect(&sim7000, 0, MQTT_HOST, MQTT_PORT))
			return false;

	//if (!MQTTIsConnected(&mqttClient) && MQTTConnect(&mqttClient, &connectData) == FAILURE)
    //    return false;


	//widecontrol.com.ar -> 107.20.89.2
	//181.170.73.71
/*	if (!sim7000_mqtt_connected(&sim7000)){
		//if (!sim7000_mqtt_config(&sim7000, "tracker10000", "107.20.89.2", 1883, "luis", "5p650jnto", 60))
		if (!sim7000_mqtt_config(&sim7000, "tracker10000", "107.20.89.2", 1883, NULL, NULL, 60))
			return false;
		//if (!sim7000_mqtt_connect(&sim7000, "datos.personal.com", 30000))
		if (!sim7000_mqtt_connect(&sim7000, NULL, 30000))
			return false;
	}*/
#endif
#ifdef SIM800
	if (!sim800_is_gprs_connected(&sim800)){
		//if (!sim7000_gprs_connect(&sim7000, "datos.personal.com", "datos","datos"))
		if (!sim800_gprs_connect(&sim800, "datos.personal.com", "datos","datos"))
			return false;
	}
/*	if (!sim800_mqtt_connected(&sim800)){
		//if (!sim7000_mqtt_config(&sim7000, "tracker10000", "107.20.89.2", 1883, "luis", "5p650jnto", 60))
		if (!sim800_mqtt_config(&sim800, "tracker10000", "107.20.89.2", 1883, NULL, NULL, 60, 20))
			return false;
		//if (!sim7000_mqtt_connect(&sim7000, "datos.personal.com", 30000))
		if (!sim800_mqtt_connect(&sim800, NULL, 30000))
			return false;
	}*/
#endif
	return true;
}
//-----------------------------------------------------------------------------
static void report_task(){
	char imei[32], imsi[32];
	char mqtt_msg;
	t_gnss_report gnss_report;
	t_gnss_report gnss_report_list[5];
	uint32_t gnss_report_count = 0;
	esp_log_level_set(REPORT_TASK_TAG, ESP_LOG_INFO);
	ESP_LOGI(REPORT_TASK_TAG, "report_task()");
	vTaskDelay(5000 / portTICK_PERIOD_MS);
	uint64_t last_report_ms = simple_timer_get_ms();
	bool first, internal_loop;
	t_simple_timer inactivity_timer;
	simple_timer_start(&inactivity_timer, utils_minutes_to_ms(5.0));
#ifdef SIM7000
	if (sim7000_setup(&sim7000))
		ESP_LOGI(MODEM_TASK_TAG, "OK!");
	if (sim7000_get_imei(&sim7000, imei))
		ESP_LOGI(MODEM_TASK_TAG, "imei: %s", imei);
	if (sim7000_get_imsi(&sim7000, imsi))
		ESP_LOGI(MODEM_TASK_TAG, "imsi: %s", imsi);
#endif
#ifdef SIM800
	if (sim800_setup(&sim800))
		ESP_LOGI(MODEM_TASK_TAG, "OK!");
	if (sim800_get_imei(&sim800, imei))
		ESP_LOGI(MODEM_TASK_TAG, "imei: %s", imei);
	if (sim800_get_imsi(&sim800, imsi))
		ESP_LOGI(MODEM_TASK_TAG, "imsi: %s", imsi);
#endif
	while (1){
#ifdef SIM7000
		ESP_LOGI(REPORT_TASK_TAG, "REPORT");
		ESP_LOGI(REPORT_TASK_TAG, "rssi(%i)", sim7000_get_rssi(&sim7000));
		ESP_LOGI(REPORT_TASK_TAG, "sim_status(%i)", sim7000_get_sim_status(&sim7000));
		ESP_LOGI(REPORT_TASK_TAG, "network_registration_status(%i)", sim7000_get_network_registration_status(&sim7000));
		ESP_LOGI(REPORT_TASK_TAG, "sim7000_get_preferred_mode(%i)", sim7000_get_preferred_mode(&sim7000));
		first = true;
		if (ready4Report()){
			internal_loop = true;
			while (internal_loop){
				ESP_LOGI(REPORT_TASK_TAG, "internal LOOP");

				//procesamiento de reportes gnss
				sim7000_gnss_report_2_gnss_report(&sim7000.last_gnss_report, &gnss_report, 10001, esp_random(), 0);
				if (gnss_report.time != gnss_report_list[4].time){
					for (int i = 0 ; i < 4 ; i++)
						gnss_report_list[i] = gnss_report_list[i + 1];
					gnss_report_list[4] = gnss_report;
					gnss_report_count++;
				}
				if (gnss_report_count >= 5){
					//detecci�n de movimiento
					ESP_LOGI(REPORT_TASK_TAG, "Listo para reportar");

					bool in_movement = true;
					for (int i = 1 ; i < 5 ; i++)
						if (gnss_report_list[i].speed < 1.0f)
							in_movement = false;
					gnss_report_list[0].in_movement = in_movement;
					if (in_movement){
						ESP_LOGI(REPORT_TASK_TAG, "RESET DE TIMER - MOVIMIENTO");
						simple_timer_reset(&inactivity_timer);
					}

					gnss_report_list[0].new_track = simple_timer_timeout(&inactivity_timer);
					//reportes
					if (simple_timer_get_ms() - last_report_ms > ((first || gnss_report_list[0].in_movement) ? 3000 : utils_hours_to_ms(1.0f))){
						ESP_LOGI(REPORT_TASK_TAG, "TIENE PARA REPORTAR");
						gnss_report_2_string(&gnss_report_list[0], &mqtt_msg);
						if (sim7000_tcp_send_frame(&sim7000, 0, &mqtt_msg, strlen(&mqtt_msg))){
						//if (sim7000_tcp_send_frame(&sim7000, 0, (char *)&gnss_report_list[0], sizeof(t_gnss_report))){
						//if (sim7000_tcp_send_frame(&sim7000, 0, "HOLA MAXI, ESTOY VIVOOOOO!!!", sizeof(t_gnss_report))){
					    //if (MQTTPublish(&mqttClient,MQTT_TOPIC, gnss_report_2_string(&gnss_report_list[0], &mqtt_msg))) {
							first = false;
							ESP_LOGI(REPORT_TASK_TAG, "REPORT OK! -> %u", gnss_report_count);
							last_report_ms = simple_timer_get_ms();
							simple_timer_reset(&inactivity_timer);
						} else
							internal_loop = false;
					}
				}
				vTaskDelay(1000 / portTICK_PERIOD_MS);
			}
		} else
			vTaskDelay(19000 / portTICK_PERIOD_MS);
#endif
		vTaskDelay(1000 / portTICK_PERIOD_MS);

		if (simple_timer_get_ms() - last_report_ms > utils_hours_to_ms(1.1f))
			esp_restart();
	}
}
//-----------------------------------------------------------------------------
/**
 * Intenta iniciar el MODEM (sim)
 * @param retrys cantidad de intentos hasta darse por vencido
 * @return Retorna true si inició correctamente
 */
bool modem_power_on(int retrys){
	while (retrys--){
#ifdef SIM7000
		if (sim7000_setup(&sim7000))
			return true;
#endif
#ifdef SIM800
		if (sim800_setup(&sim800))
			return true;
#endif
		gpio_set_level(MODEM_PWRKEY_PIN, 1);
		vTaskDelay(1100 / portTICK_PERIOD_MS);
		gpio_set_level(MODEM_PWRKEY_PIN, 0);
		vTaskDelay(100 / portTICK_PERIOD_MS);
		vTaskDelay(6000 / portTICK_PERIOD_MS);
	}
	return false;
}
//-----------------------------------------------------------------------------
static void modem_core_task(){
    esp_log_level_set(MODEM_CORE_TASK_TAG, ESP_LOG_INFO);
    ESP_LOGI(MODEM_CORE_TASK_TAG, "modem_core_task()");
    while (1) {
    	//sim7000_core_poll(&sim7000_core);
#ifdef SIM7000
    	sim_core_poll(&sim7000.core);
#endif
#ifdef SIM800
    	sim_core_poll(&sim800.core);
#endif
    	vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}
//-----------------------------------------------------------------------------
void modem_init() {
    // Configuración GPIO
    gpio_config_t io_conf = {
        .intr_type     = GPIO_PIN_INTR_DISABLE,
        .mode         = GPIO_MODE_OUTPUT,
        .pin_bit_mask = (1 << MODEM_SLEEP_GPS_ENABLE_PIN) | (1 << MODEM_PWRKEY_PIN),
        .pull_down_en = 0,
        .pull_up_en   = 0
    };
    gpio_config(&io_conf);

    gpio_set_level(MODEM_PWRKEY_PIN, 0);
#ifdef SIM7000
    //gpio_set_level(MODEM_SLEEP_GPS_ENABLE_PIN, 0);
    gpio_set_level(MODEM_SLEEP_GPS_ENABLE_PIN, 1);
#endif
#ifdef SIM800
    gpio_set_level(MODEM_SLEEP_GPS_ENABLE_PIN, 1);
#endif

    const uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    uart_param_config(UART_NUM_1, &uart_config);
    uart_set_pin(UART_NUM_1, MODEM_TXD_PIN, MODEM_RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    // We won't use a buffer for sending data.
    uart_driver_install(UART_NUM_1, 1024 * 2, 0, 0, NULL, 0);

#ifdef SIM7000
    sim7000_init(&sim7000, UART_NUM_1, 10);
#endif
#ifdef SIM800
    sim800_init(&sim800, UART_NUM_1, 10);
#endif
    xTaskCreate(modem_core_task, "MODEM_CORE_TASK", 4096, NULL, configMAX_PRIORITIES, NULL);
    if (!modem_power_on(3)){
    	esp_restart();
    }
}
//-----------------------------------------------------------------------------
static void modem_task(){
	//bool ok, error;
	//char response[1024];
//	char imei[16];
//	char imsi[16];
    esp_log_level_set(MODEM_TASK_TAG, ESP_LOG_INFO);
    ESP_LOGI(MODEM_TASK_TAG, "modem_task()");
    modem_init();
    xTaskCreate(report_task, "REPORT_TASK", 4096, NULL, configMAX_PRIORITIES, NULL);
//    vTaskDelay(5000 / portTICK_PERIOD_MS);
/*    if (sim7000_setup(&sim7000))
    	ESP_LOGI(MODEM_TASK_TAG, "OK!");
    if (sim7000_get_imei(&sim7000, imei))
    	ESP_LOGI(MODEM_TASK_TAG, "imei: %s", imei);
    if (sim7000_get_imsi(&sim7000, imsi))
    	ESP_LOGI(MODEM_TASK_TAG, "imsi: %s", imsi);*/
/*    sim7000_core_send_at_and_wait_for_response(&sim7000_core, "AT\r", response, 1000, &ok, &error);
    ESP_LOGI(MODEM_TASK_TAG, "sim7000_core_send_at_and_wait_for_response(ok=%d error=%d)\n%s", ok, error, response);
    sim7000_core_send_at_and_wait_for_response(&sim7000_core, "AT+CGNSPWR=1\r", response, 1000, &ok, &error);
    ESP_LOGI(MODEM_TASK_TAG, "sim7000_core_send_at_and_wait_for_response(ok=%d error=%d)\n%s", ok, error, response);
    sim7000_core_send_at_and_wait_for_response(&sim7000_core, "AT+CGNSURC=1\r", response, 1000, &ok, &error);
    ESP_LOGI(MODEM_TASK_TAG, "sim7000_core_send_at_and_wait_for_response(ok=%d error=%d)\n%s", ok, error, response);*/
    while (1) {
#ifdef SIM7000
    	sim7000_poll(&sim7000);
#endif
#ifdef SIM800
    	sim800_poll(&sim800);
#endif
    	//ESP_LOGI(MODEM_TASK_TAG, "MODEM_TASK_LOOP");
//    	if (sim7000_core_urc_received(&sim7000_core, response))
//    		ESP_LOGI(MODEM_TASK_TAG, "sim7000_core_urc_received()\n%s", response);
    	vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
//-----------------------------------------------------------------------------
void status_init(){
	ESP_LOGI(STATUS_TASK_TAG, "status_init()");

	// Configuración GPIO
    gpio_config_t io_conf = {
        .intr_type    = GPIO_PIN_INTR_DISABLE,
        .mode         = GPIO_MODE_INPUT_OUTPUT,
        .pin_bit_mask = 1 << BLINK_GPIO,
        .pull_down_en = 0,
        .pull_up_en   = 0
    };

    ESP_LOGI(STATUS_TASK_TAG, "gpio_config() -> %i", gpio_config(&io_conf));
}
//-----------------------------------------------------------------------------
static void status_task(){
	esp_log_level_set(STATUS_TASK_TAG, ESP_LOG_INFO);
	ESP_LOGI(STATUS_TASK_TAG, "status_task()");
	status_init();
    while (1){
    	if (simple_timer_get_ms() > utils_hours_to_ms(12.0f))
    		esp_restart();
//    	ESP_LOGI(STATUS_TASK_TAG, "STATUS -> %i", );
    	gpio_set_level(BLINK_GPIO, gpio_get_level(BLINK_GPIO) ^ 1);
    	vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
//-----------------------------------------------------------------------------
int MQTTWrite(Network* n, unsigned char* buffer, int len, int timeout_ms) {
    bool success = sim7000_tcp_send_frame(&sim7000, 0, (char *) buffer, len);
    return success ? SUCCESS : FAILURE;
}

void MQTTMyDisconnect(Network* n) {
    sim7000_tcp_disconnect(&sim7000, 0);
}

void MQTTInit() {
    network.my_socket = 0;
    network.mqttwrite = MQTTWrite;
    network.disconnect = MQTTMyDisconnect;

    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "MiCliente";

    MQTTClientInit(&mqttClient, &network, 30000, mqtt_sendbuf, sizeof(mqtt_sendbuf), mqtt_readbuf, sizeof(mqtt_readbuf));
}

void app_main(){
    //MQTTInit();
    xTaskCreate(modem_task,  "MODEM_TASK",  4096, NULL, configMAX_PRIORITIES, NULL);
    xTaskCreate(status_task, "STATUS_TASK", 2048, NULL, configMAX_PRIORITIES, NULL);
}
//-----------------------------------------------------------------------------
