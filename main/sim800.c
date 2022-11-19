/*
 * sim800.c
 *
 * Created: 12/06/2020 23:47 p.m.
 *  Author: Luis Pichio
 */

#include "sim800.h"

#define DEBUG_SIM800
const char* SIM800_TAG = "SIM800";

//------------------------------------------------------------------------------
//Unsolicited Result Codes
const char c_urc_0[] = "+CRING";
const char c_urc_1[] = "+CREG";
const char c_urc_2[] = "+CMTI";
const char c_urc_3[] = "+CMT";
const char c_urc_4[] = "+CBM";
const char c_urc_5[] = "+CDS";
const char c_urc_6[] = "*PSNWID";
const char c_urc_7[] = "*PSUTTZ";
const char c_urc_8[] = "+CTZV";
const char c_urc_9[] = "DST";
const char c_urc_10[] = "+CPIN";
const char c_urc_11[] = "+CUSD";
const char c_urc_12[] = "NORMAL POWER DOWN";
const char c_urc_13[] = "UNDER-VOLTAGE POWER DOWN";
const char c_urc_14[] = "UNDER-VOLTAGE WARNNING";
const char c_urc_15[] = "OVER-VOLTAGE POWER DOWN";
const char c_urc_16[] = "OVER-VOLTAGE WARNNING";
const char c_urc_17[] = "RDY";
const char c_urc_18[] = "+CFUN";
const char c_urc_19[] = "CONNECT OK";
const char c_urc_20[] = "CONNECT";
const char c_urc_21[] = "CONNECT FAIL";
const char c_urc_22[] = "ALREADY CONNECT";
const char c_urc_23[] = "SEND OK";
const char c_urc_24[] = "CLOSED";
const char c_urc_25[] = "RECV FROM";
const char c_urc_26[] = "+IPD";
const char c_urc_27[] = "+RECEIVE";
const char c_urc_28[] = "REMOTE IP";
const char c_urc_29[] = "+CDNSGIP";
const char c_urc_30[] = "+PDP";
const char c_urc_31[] = "+APP PDP";
const char c_urc_32[] = "+UGNSINF";
const char c_urc_33[] = "+CGNSINF";

/*
const char c_urc_0[] = "+CCWA";
const char c_urc_1[] = "CLIP";
const char c_urc_2[] = "CRING";
const char c_urc_3[] = "+CREG";
const char c_urc_4[] = "+CCWV";
const char c_urc_5[] = "+CMTI";
const char c_urc_6[] = "+CMT";
const char c_urc_7[] = "+CBM";
const char c_urc_8[] = "+CDS";
const char c_urc_9[] = "+COLP";
const char c_urc_10[] = "+CSSU";
const char c_urc_11[] = "+CSSI";
const char c_urc_12[] = "+CLCC";
const char c_urc_13[] = "*PSNWID";
const char c_urc_14[] = "*PSUTTZ";
const char c_urc_15[] = "+CTZV";
const char c_urc_16[] = "DST";
const char c_urc_17[] = "+CSMINS";
const char c_urc_18[] = "+CDRIND";
const char c_urc_19[] = "+CHF";
const char c_urc_20[] = "+CENG";
const char c_urc_21[] = "MO RING";
const char c_urc_22[] = "MO CONNECTED";
const char c_urc_23[] = "+CPIN";
const char c_urc_24[] = "+CSQN";
const char c_urc_25[] = "+SIMTONE";
const char c_urc_26[] = "+STTONE";
const char c_urc_27[] = "+CR";
const char c_urc_28[] = "+CUSD";
const char c_urc_29[] = "RING";
const char c_urc_30[] = "NORMAL POWER DOWN";
const char c_urc_31[] = "+CMTE";
const char c_urc_32[] = "UNDER-VOLTAGE POWER DOWN";
const char c_urc_33[] = "UNDER-VOLTAGE WARNNING";
const char c_urc_34[] = "OVER-VOLTAGE POWER DOWN";
const char c_urc_35[] = "OVER-VOLTAGE WARNNING";
const char c_urc_36[] = "CHARGE-ONLY MODE";
const char c_urc_37[] = "RDY";
const char c_urc_38[] = "Call Ready";
const char c_urc_39[] = "SMS Ready";
const char c_urc_40[] = "+CFUN";
const char c_urc_41[] = "CONNECT OK";
const char c_urc_42[] = "CONNECT";
const char c_urc_43[] = "CONNECT FAIL";
const char c_urc_44[] = "ALREADY CONNECT";
const char c_urc_45[] = "SEND OK";
const char c_urc_46[] = "CLOSED";
const char c_urc_47[] = "RECV FROM";
const char c_urc_48[] = "+IPD";
const char c_urc_49[] = "+RECEIVE";
const char c_urc_50[] = "REMOTE IP";
const char c_urc_51[] = "+CDNSGIP";
const char c_urc_52[] = "+PDP DEACT";
const char c_urc_53[] = "+SAPBR";
const char c_urc_54[] = "+HTTPACTION";
const char c_urc_55[] = "+FTPGET";
const char c_urc_56[] = "+FTPPUT";
const char c_urc_57[] = "+FTPDELE";
const char c_urc_58[] = "+FTPSIZE";
const char c_urc_59[] = "+FTPMKD";
const char c_urc_60[] = "+FTPRMD";
const char c_urc_61[] = "+FTPLIST";
const char c_urc_62[] = "+UGNSINF";
*/

const char *c_urc[] = {
	c_urc_0,
	c_urc_1,
	c_urc_2,
	c_urc_3,
	c_urc_4,
	c_urc_5,
	c_urc_6,
	c_urc_7,
	c_urc_8,
	c_urc_9,
	c_urc_10,
	c_urc_11,
	c_urc_12,
	c_urc_13,
	c_urc_14,
	c_urc_15,
	c_urc_16,
	c_urc_17,
	c_urc_18,
	c_urc_19,
	c_urc_20,
	c_urc_21,
	c_urc_22,
	c_urc_23,
	c_urc_24,
	c_urc_25,
	c_urc_26,
	c_urc_27,
	c_urc_28,
	c_urc_29,
	c_urc_30,
	c_urc_31,
	c_urc_32,
	c_urc_33,
	NULL
};
//------------------------------------------------------------------------------
const char c_sim7000_setup_cmd_0[] = "AT+CGMR\r";  //TA Revision Identification of Software Release
const char c_sim7000_setup_cmd_1[] = "AT+CFUN=1\r";
const char c_sim7000_setup_cmd_2[] = "AT+CGNSPWR=1\r";
const char c_sim7000_setup_cmd_3[] = "AT+CGNSURC=1\r";
//------------------------------------------------------------------------------
const char* c_sim800_setup_cmd[] = {
	c_sim7000_setup_cmd_0,
	c_sim7000_setup_cmd_1,
	c_sim7000_setup_cmd_2,
	c_sim7000_setup_cmd_3,
	NULL,
};
//------------------------------------------------------------------------------
bool sim800_setup(t_sim800 *sim800){
	int cmd = 0;
	bool ok, error, timeout;
	char response[256];
#ifdef DEBUG_SIM800
	ESP_LOGI(SIM800_TAG, "sim800_setup()");
#endif
	sim_core_send_at_and_wait_for_response(&sim800->core, "ATE0\r", response, 2000, &ok, &error, &timeout);
	sim_core_send_at_and_wait_for_response(&sim800->core, "ATE0\r", response, 2000, &ok, &error, &timeout);
	while (c_sim800_setup_cmd[cmd] != NULL){
		if  (sim_core_send_at_and_wait_for_response(&sim800->core, c_sim800_setup_cmd[cmd], response, 1000, &ok, &error, &timeout) && ok && !error && !timeout)
			cmd++;
		else
			return false;
	}
	return true;
}
//------------------------------------------------------------------------------
bool sim800_valid_imei_imsi(char *imei_imsi){
	if (strlen(imei_imsi) > 15)
		return false;
	for (int i = 0 ; i < strlen(imei_imsi) ; i++)
		if (!isdigit(imei_imsi[i]))
			return false;
	return true;
}
//------------------------------------------------------------------------------
bool is_a_number(char *s){
	while (*s)
		if (!isdigit(*s))
			return false;
		else
			s++;
	return true;
}
//------------------------------------------------------------------------------
bool sim800_get_imei(t_sim800 *sim800, char *imei){
	char response[256];
	bool ok, error, timeout;
	char *value;
	strcpy(imei, "");
	if (!sim_core_send_at_and_wait_for_response(&sim800->core, "AT+GSN\r", response, 1000, &ok, &error, &timeout) || error)
		return false;
//	ESP_LOGI(SIM800_TAG, "response: %s", response);
	value = strtok(response, CRLF);
//	ESP_LOGI(SIM800_TAG, "value: %s", value);
	if (!is_a_number(value) || strlen(value) > 15)
		return false;
	strcpy(imei, value);
	return true;
}
//------------------------------------------------------------------------------
bool sim800_get_imsi(t_sim800 *sim800, char *imsi){
	char response[256];
	bool ok, error, timeout;
	char *value;
	strcpy(imsi, "");
	if (!sim_core_send_at_and_wait_for_response(&sim800->core, "AT+CIMI\r", response, 1000, &ok, &error, &timeout) || error)
		return false;
//	ESP_LOGI(SIM800_TAG, "response: %s", response);
	value = strtok(response, CRLF);
//	ESP_LOGI(SIM800_TAG, "value: %s", value);
	if (!is_a_number(value) || strlen(value) > 15)
		return false;
	strcpy(imsi, value);
	return true;
}
//------------------------------------------------------------------------------
bool sim800_get_rssi_ber(t_sim800 *sim800, int16_t *rssi, int16_t *ber){
	char response[256];
	bool ok, error, timeout;
	char *p;
	*rssi = 99;
	*ber = 99;
	if (!sim_core_send_at_and_wait_for_response(&sim800->core, "AT+CSQ\r", response, 1000, &ok, &error, &timeout) || error || timeout)
		return false;
	if ((p = strstr(response, "+CSQ: "))){
		p += strlen("+CSQ: ");
		*rssi = sim_core_rssi_to_dbm(utils_tokenl(p, ',', 0));
		*ber = utils_tokenl(p, ',', 1);
		return true;
	}
	return false;
}
//------------------------------------------------------------------------------
int16_t sim800_get_rssi(t_sim800 *sim800){
	int16_t rssi, ber;
	if (sim800_get_rssi_ber(sim800, &rssi, &ber))
		return rssi;
	return INT16_MIN;
}
//------------------------------------------------------------------------------
t_sim800_sim_status sim800_get_sim_status(t_sim800 *sim800){
	bool ok, error, timeout;
	char response[512];
	if (!sim_core_send_at_and_wait_for_response(&sim800->core, "AT+CPIN?\r", response, 1000, &ok, &error, &timeout) || !ok || error || timeout)
		return SIM800_SIM_STATUS_UNKNOWN;
	if (strstr(response, "READY"))
		return SIM800_SIM_STATUS_READY;
	if (strstr(response, "SIM PIN") || strstr(response, "SIM PUK"))
		return SIM800_SIM_STATUS_LOCKED;
	if (strstr(response, "PH_SIM PIN") || strstr(response, "PH_SIM PUK"))
		return SIM800_SIM_STATUS_ANTITHEFT_LOCKED;
	return SIM800_SIM_STATUS_ERROR;
}
//------------------------------------------------------------------------------
t_sim800_registration_status sim800_get_network_registration_status(t_sim800 *sim800) {
	bool ok, error, timeout;
	char response[256];
	char *p;
	if (!sim_core_send_at_and_wait_for_response(&sim800->core, "AT+CGREG?\r", response, 1000, &ok, &error, &timeout) || !ok || error || timeout)
		return SIM800_REGISTRATION_STATUS_UNKNOWN;
	if ((p = strstr(response, "+CGREG: "))){
		p += strlen("+CGREG: ");
		switch (utils_tokenl(p, ',', 1)){
			case 0:
				return SIM800_REGISTRATION_STATUS_NOT_REGISTERED;
			case 1:
				return SIM800_REGISTRATION_STATUS_REGISTERED_HOME;
			case 2:
				return SIM800_REGISTRATION_STATUS_SEARCHING;
			case 3:
				return SIM800_REGISTRATION_STATUS_DENIED;
			case 4:
				return SIM800_REGISTRATION_STATUS_UNKNOWN;
			case 5:
				return SIM800_REGISTRATION_STATUS_REGISTERED_ROAMING;
		}
	}
	return SIM800_REGISTRATION_STATUS_UNKNOWN;
}
//------------------------------------------------------------------------------
/*t_registration_status sim800_get_network_mode_operator(t_sim800 *sim800) {
	bool ok, error, timeout;
	char response[256];
	char stat[16];
	char *p;
	if (!sim_core_send_at_and_wait_for_response(&sim800->core, "AT+COPS?\r", response, 1000, &ok, &error, &timeout) || !ok || error || timeout)
		return REGISTRATION_STATUS_UNKNOWN;
	if ((p = strstr(response, "+CGREG: "))){
		p += strlen("+CGREG: ");
		switch (utils_tokenl(p, ',', 1)){
			case 0:
				return REGISTRATION_STATUS_NOT_REGISTERED;
			case 1:
				return REGISTRATION_STATUS_REGISTERED_HOME;
			case 2:
				return REGISTRATION_STATUS_SEARCHING;
			case 3:
				return REGISTRATION_STATUS_DENIED;
			case 4:
				return REGISTRATION_STATUS_UNKNOWN;
			case 5:
				return REGISTRATION_STATUS_REGISTERED_ROAMING;
		}
	}
	return REGISTRATION_STATUS_UNKNOWN;
}*/
//------------------------------------------------------------------------------
t_sim800_preferred_mode sim800_get_preferred_mode(t_sim800 *sim800) {
	bool ok, error, timeout;
	char response[256];
	char *p;
	if (!sim_core_send_at_and_wait_for_response(&sim800->core, "AT+CNMP?\r", response, 1000, &ok, &error, &timeout) || !ok || error || timeout)
		return SIM800_PREFERRED_MODE_UNKNOWN;
	if ((p = strstr(response, "+CNMP: "))){
		p += strlen("+CNMP: ");
		switch (utils_tokenl(p, ',', 0)){
			case 2:
				return SIM800_PREFERRED_MODE_AUTOMATIC;
			case 13:
				return SIM800_PREFERRED_MODE_GSM_ONLY;
			case 38:
				return SIM800_PREFERRED_MODE_LTE_ONLY;
			case 51:
				return SIM800_PREFERRED_MODE_GSM_AND_LTE_ONLY;
		}
	}
	return SIM800_PREFERRED_MODE_UNKNOWN;
}
//------------------------------------------------------------------------------
bool sim800_get_network_apn(t_sim800 *sim800, char *apn) {
	bool ok, error, timeout;
	char response[256];
	char *p;
	if (!sim_core_send_at_and_wait_for_response(&sim800->core, "AT+CGNAPN\r", response, 1000, &ok, &error, &timeout) || !ok || error || timeout)
		return false;
	if ((p = strstr(response, "+CGNAPN: "))){
		p += strlen("+CGNAPN: ");
		if (utils_tokenl(p, ',', 0)){
			utils_token(p, ',', 1, apn, 31);
			return true;
		}
	}
	return false;
}
//------------------------------------------------------------------------------
bool sim800_is_gprs_connected(t_sim800 *sim800) {
	bool ok, error, timeout;
	char response_ip[256];
	char *state;
	if (!sim_core_send_at_and_wait_for_response(&sim800->core, "AT+CGATT?\r", response_ip, 1000L, &ok, &error, &timeout) || !ok || error || timeout)
		return false;
	if ((state = strstr(response_ip, "+CGATT: "))){
		state += strlen("+CGATT: ");
		if (strlen(state) && (atoi(state) != 1))
			return false;
	}
	return sim800_get_local_ip(sim800, response_ip) && strstr(response_ip, "0.0.0.0") == NULL;
}
//------------------------------------------------------------------------------
/*
// Gets the current network operator via the 3GPP TS command AT+COPS
String getOperatorImpl() {
  thisModem().sendAT(GF("+COPS?"));
  if (thisModem().waitResponse(GF("+COPS:")) != 1) { return ""; }
  thisModem().streamSkipUntil('"'); // Skip mode and format
  String res = thisModem().stream.readStringUntil('"');
  thisModem().waitResponse();
  return res;
}
*/
//------------------------------------------------------------------------------
bool sim800_get_local_ip(t_sim800 *sim800, char *ip) {
	bool ok, error, timeout;
	char response[256];
	if (!sim_core_send_at_and_wait_for_response(&sim800->core, "AT+CIFSR\r", response, 10000L, &ok, &error, &timeout) || error || timeout)
		return false;
	strcpy(ip, response);
	return true;
}
//------------------------------------------------------------------------------
bool sim800_gprs_connect(t_sim800 *sim800, char *apn, char *user, char *password) {
	bool ok, error, timeout;
	char response[256];
	char str[128];
	sim800_gprs_disconnect(sim800);
	 //set the connection type to GPRS
	if (!sim_core_send_at_and_wait_for_response(&sim800->core, "AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"\r", response, 1000, &ok, &error, &timeout) || !ok || error || timeout)
		return false;
	//set the APN
	sprintf(str, "AT+SAPBR=3,1,\"APN\",\"%s\"\r", apn);
	if (!sim_core_send_at_and_wait_for_response(&sim800->core, str, response, 1000, &ok, &error, &timeout) || !ok || error || timeout)
		return false;
	//set the user name
	if (user){
		sprintf(str, "AT+SAPBR=3,1,\"USER\",\"%s\"\r", user);
		if (!sim_core_send_at_and_wait_for_response(&sim800->core, str, response, 1000, &ok, &error, &timeout) || !ok || error || timeout)
			return false;
	}
	if (password){
		//set the password
		sprintf(str, "AT+SAPBR=3,1,\"PWD\",\"%s\"\r", password);
		if (!sim_core_send_at_and_wait_for_response(&sim800->core, str, response, 1000, &ok, &error, &timeout) || !ok || error || timeout)
			return false;
	}
	//define the PDP context
	sprintf(str, "AT+CGDCONT=1,\"IP\",\"%s\"\r", apn);
	if (!sim_core_send_at_and_wait_for_response(&sim800->core, str, response, 1000, &ok, &error, &timeout) || !ok || error || timeout)
		return false;
	//activate the PDP context
	if (!sim_core_send_at_and_wait_for_response(&sim800->core, "AT+CGACT=1,1\r", response, 60000L, &ok, &error, &timeout) || !ok || error || timeout)
		return false;
	//open the definied GPRS bearer context
	if (!sim_core_send_at_and_wait_for_response(&sim800->core, "AT+SAPBR=1,1\r", response, 85000L, &ok, &error, &timeout) || !ok || error || timeout)
		return false;
	//query the GPRS bearer context status
	if (!sim_core_send_at_and_wait_for_response(&sim800->core, "AT+SAPBR=2,1\r", response, 30000L, &ok, &error, &timeout) || !ok || error || timeout)
		return false;
	//attach to GPRS
	if (!sim_core_send_at_and_wait_for_response(&sim800->core, "AT+CGATT=1\r", response, 60000L, &ok, &error, &timeout) || !ok || error || timeout)
		return false;
	// TODO(?): wait AT+CGATT?
	//set to multi-IP
	if (!sim_core_send_at_and_wait_for_response(&sim800->core, "AT+CIPMUX=1\r", response, 1000, &ok, &error, &timeout) || !ok || error || timeout)
		return false;
	//put in "quick send" mode (thus no extra "Send OK")
	if (!sim_core_send_at_and_wait_for_response(&sim800->core, "AT+CIPQSEND=1\r", response, 1000, &ok, &error, &timeout) || !ok || error || timeout)
		return false;
	//set to get data manually
	if (!sim_core_send_at_and_wait_for_response(&sim800->core, "AT+CIPRXGET=1\r", response, 1000, &ok, &error, &timeout) || !ok || error || timeout)
		return false;
	//disable '>' prompt on AT+CIPSEND
/*	if (!sim_core_send_at_and_wait_for_response(&sim800->core, "AT+CIPSPRT=0\r", response, 1000L, &ok, &error, &timeout) || !ok || error || timeout)
		return false;*/
	//start task and set APN, USER NAME, PASSWORD
	sprintf(str, "AT+CSTT=\"%s\",\"%s\",\"%s\"\r", apn, user, password);
	if (!sim_core_send_at_and_wait_for_response(&sim800->core, str, response, 60000L, &ok, &error, &timeout) || !ok || error || timeout)
		return false;
	//bring up wireless connection with GPRS or CSD
	if (!sim_core_send_at_and_wait_for_response(&sim800->core, "AT+CIICR\r", response, 60000L, &ok, &error, &timeout) || !ok || error || timeout)
		return false;
	//get local IP address, only assigned after connection
	return sim800_get_local_ip(sim800, str) && strstr(str, "0.0.0.0") == NULL;
	return true;
}
//------------------------------------------------------------------------------
bool sim800_gprs_disconnect(t_sim800 *sim800) {
	bool ok, error, timeout;
	char response[256];
	//shut the TCP/IP connection
	//CIPSHUT will close *all* open connections
	if (!sim_core_send_at_and_wait_for_response(&sim800->core, "AT+CIPSHUT\r", response, 60000, &ok, &error, &timeout) || error || timeout)
		return false;
	if (strstr(response, "SHUT OK") == NULL)
		return false;
	//deactivate the bearer context
	if (!sim_core_send_at_and_wait_for_response(&sim800->core, "AT+CGATT=0\r", response, 60000, &ok, &error, &timeout) || !ok || error || timeout)
		return false;
	return true;
}
//------------------------------------------------------------------------------
/*
String geNetworkModes() {
  sendAT(GF("+CNMP=?"));
  if (waitResponse(GF(GSM_NL "+CNMP:")) != 1) { return ""; }
  String res = stream.readStringUntil('\n');
  waitResponse();
  return res;
}

String setNetworkMode(uint8_t mode) {
  sendAT(GF("+CNMP="), mode);
  if (waitResponse(GF(GSM_NL "+CNMP:")) != 1) { return "OK"; }
  String res = stream.readStringUntil('\n');
  waitResponse();
  return res;
}

String getPreferredModes() {
  sendAT(GF("+CMNB=?"));
  if (waitResponse(GF(GSM_NL "+CMNB:")) != 1) { return ""; }
  String res = stream.readStringUntil('\n');
  waitResponse();
  return res;
}

String setPreferredMode(uint8_t mode) {
  sendAT(GF("+CMNB="), mode);
  if (waitResponse(GF(GSM_NL "+CMNB:")) != 1) { return "OK"; }
  String res = stream.readStringUntil('\n');
  waitResponse();
  return res;
}
*/
//------------------------------------------------------------------------------
bool sim800_mqtt_config(t_sim800 *sim800, char* client_id, char* host, int port, char* user_name, char* password, int keep_time, int mqtt_response_timeout){
	bool ok, error, timeout;
	char str[128];
	char response[256];
#ifdef DEBUG_SIM800
	ESP_LOGI(SIM800_TAG, "sim800_mqtt_config()");
#endif
/*	sprintf(str, "AT+SMCONF=\"CLIENTID\",\"%s\"\r", client_id);
	if (!sim_core_send_at_and_wait_for_response(&sim800->core, str, response, 1000, &ok, &error, &timeout) || !ok || error || timeout)
		return false;*/
	//sprintf(str, "AT+SMCONF=\"URL\",\"%s\",\"%i\"\r", host, port);
	sprintf(str, "AT+SMCONF=\"URL\",\"%s,%i\"\r", host, port);
	if (!sim_core_send_at_and_wait_for_response(&sim800->core, str, response, 1000, &ok, &error, &timeout) || !ok || error || timeout)
		return false;
	if (user_name){
		sprintf(str, "AT+SMCONF=\"USERNAME\",\"%s\"\r", user_name);
		if (!sim_core_send_at_and_wait_for_response(&sim800->core, str, response, 1000, &ok, &error, &timeout) || !ok || error || timeout)
			return false;
	}
	if (password){
		sprintf(str, "AT+SMCONF=\"PASSWORD\",\"%s\"\r", password);
		if (!sim_core_send_at_and_wait_for_response(&sim800->core, str, response, 1000, &ok, &error, &timeout) || !ok || error || timeout)
			return false;
	}
	sprintf(str, "AT+SMCONF=\"KEEPALIVE\",\"%u\"\r", keep_time);
	if (!sim_core_send_at_and_wait_for_response(&sim800->core, str, response, 1000, &ok, &error, &timeout) || !ok || error || timeout)
		return false;
	sprintf(str, "AT+SMCONF=\"TIMEOUT\",\"%u\"\r", mqtt_response_timeout);
	if (!sim_core_send_at_and_wait_for_response(&sim800->core, str, response, 1000, &ok, &error, &timeout) || !ok || error || timeout)
		return false;
	if (!sim_core_send_at_and_wait_for_response(&sim800->core, "AT+SMCONF=\"CLEANSS\",1\r", response, 1000, &ok, &error, &timeout) || !ok || error || timeout)
		return false;
	return true;
}
//------------------------------------------------------------------------------
bool sim800_mqtt_connect(t_sim800 *sim800, char *apn, uint32_t response_timeout){
	bool ok, error, timeout;
	char response[256];
	char str[128];
	//app network active
	if (apn){
		sprintf(str, "AT+CNACT=1,\"%s\"\r", apn);
		if (!sim_core_send_at_and_wait_for_response(&sim800->core, str, response, 10000L, &ok, &error, &timeout) || !ok || error || timeout)
			return false;
	}
	return sim_core_send_at_and_wait_for_response(&sim800->core, "AT+SMCONN\r", response, response_timeout, &ok, &error, &timeout) && ok;
}
//------------------------------------------------------------------------------
bool sim800_mqtt_disconnect(t_sim800 *sim800){
	bool ok, error, timeout;
	char response[256];
	return sim_core_send_at_and_wait_for_response(&sim800->core, "AT+SMDISC\r", response, 1000, &ok, &error, &timeout) && ok;
}
//------------------------------------------------------------------------------
bool sim800_mqtt_connected(t_sim800 *sim800){
	bool ok, error, timeout;
	char response[256];
	char *state;
	if (!sim_core_send_at_and_wait_for_response(&sim800->core, "AT+SMSTATE?\r", response, 1000, &ok, &error, &timeout) || !ok || error || timeout)
		return false;
	if ((state = strstr(response, "+SMSTATE: "))){
		state += strlen("+SMSTATE: ");
		return strlen(state) && (atoi(state) == 1);
	}
	return false;
}
//------------------------------------------------------------------------------
bool sim800_mqtt_pub(t_sim800 *sim800, char *topic, char* content, int qos, int retain){
	bool ok, error, timeout;
	char response[256];
	char str[512];
	sprintf(str, "AT+SMPUB=\"%s\",\"%i\",%i,%i\r", topic, strlen(content), qos, retain);
	if (!sim_core_send_at_and_wait_for_response(&sim800->core, str, response, 1000, &ok, &error, &timeout) || !ok || error || timeout)
		return false;
	return sim_core_send(&sim800->core, content);
}
//------------------------------------------------------------------------------
bool sim800_mqtt_sub(t_sim800 *sim800, char* topic, int qos){
	bool ok, error, timeout;
	char str[128];
	char response[256];
	sprintf(str, "AT+SMSUB=\"%s\",%i\r", topic, qos);
	return sim_core_send_at_and_wait_for_response(&sim800->core, str, response, 1000, &ok, &error, &timeout) && ok;
}
//------------------------------------------------------------------------------
bool sim800_mqtt_unsub(t_sim800 *sim800, char* topic){
	bool ok, error, timeout;
	char str[128];
	char response[256];
	sprintf(str, "AT+SMUNSUB=\"%s\"\r", topic);
	return sim_core_send_at_and_wait_for_response(&sim800->core, str, response, 1000, &ok, &error, &timeout) && ok;
}
//------------------------------------------------------------------------------
/*bool sim800_http_result_from_urc(char *urc, uint16_t *method, uint16_t *status_code, uint16_t *data_len){
	char *action;
	if (action = strstr_P(urc, PSTR("+HTTPACTION:"))){
		strtok(action, ":");
		*method = atoi(strtok(NULL, ","));
		*status_code = atoi(strtok(NULL, ","));
		*data_len = atoi(strtok(NULL, ","));
		return true;
	}
	return false;
}
//------------------------------------------------------------------------------
bool sim800_http_post(t_sim800 *sim800, char *url, char *content, char *response, uint16_t max_response_size){
	char str[256];
	uint16_t method, status_code, data_len;
	WDT_Reset();
	if (!sim800_send_at_and_wait_for_ok(sim800, strcpy_P(str, PSTR("AT+HTTPPARA=\"CID\",1\r")), 500))
		return false;
	WDT_Reset();
	sprintf_P(str, PSTR("AT+HTTPPARA=\"URL\",\"%s\"\r"), url);		
	if (!sim800_send_at_and_wait_for_ok(sim800, str, 500))
		return false;
	WDT_Reset();
	if (!sim800_send_at_and_wait_for_ok(sim800, strcpy_P(str, PSTR("AT+HTTPPARA=\"CONTENT\",\"application/x-www-form-urlencoded\"\r")), 500))
		return false;
	WDT_Reset();
	sprintf_P(str, PSTR("AT+HTTPDATA=%u,%u\r"), strlen(content), 5000);
	if (!sim800_send_at_and_wait_for_response(sim800, str, str, max_response_size, 5000))
		return false;		
	if (strstr_P(str, PSTR("DOWNLOAD")) == NULL)
		return false;
	WDT_Reset();
	if (!sim800_send_at_and_wait_for_response(sim800, content, response, max_response_size, 2000))
		return false;
	WDT_Reset();
	if (!sim800_send_at_and_wait_for_ok(sim800, strcpy_P(str, PSTR("AT+HTTPACTION=1\r")), 5000))
		return false;
	WDT_Disable();
	if (!sim800_wait_for_urc(sim800, str, 256, utils_seconds_to_ms(20))){
		WDT_EnableAndSetTimeout(WDT_PER_8KCLK_gc);
		return false;
	}
	WDT_EnableAndSetTimeout(WDT_PER_8KCLK_gc);
	if (!sim800_http_result_from_urc(str, &method, &status_code, &data_len))
		return false;
	if ((status_code != 200) || (data_len > (max_response_size - 1)))
		return false;
	WDT_Reset();
	if (!sim800_send_at_and_wait_for_response(sim800, strcpy_P(str, PSTR("AT+HTTPREAD\r")), response, max_response_size, 5000))
		return false;		
	if (strstr(response, strcpy_P(str, PSTR("+HTTPREAD:"))) == NULL)
		return false;
	if (!sim800_wait_for_data(sim800, response, data_len, 5000))
		return false;
	response[data_len] = 0;
#ifdef DEBUG_MODE
	comm_println(&DEBUG_USART, response);
#endif
	return true;
}
//------------------------------------------------------------------------------*/
bool sim800_http_get(t_sim800 *sim800, char *url, char *response, uint16_t max_response_size){
	bool ok, error, timeout;
	char str[256];
//	uint16_t method, status_code, data_len;
	if (!sim_core_send_at_and_wait_for_response(&sim800->core, "AT+HTTPPARA=\"CID\",1\r", response, 1000, &ok, &error, &timeout) || !ok || error || timeout)
		return false;
	sprintf(str, "AT+HTTPPARA=\"URL\",\"%s\"\r", url);
	if (!sim_core_send_at_and_wait_for_response(&sim800->core, str, response, 1000, &ok, &error, &timeout) || !ok || error || timeout)
		return false;
	if (!sim_core_send_at_and_wait_for_response(&sim800->core, "AT+HTTPACTION=0\r", response, 30000, &ok, &error, &timeout) || !ok || error || timeout)
		return false;
/*
	WDT_EnableAndSetTimeout(WDT_PER_8KCLK_gc);
	if (!sim800_http_result_from_urc(str, &method, &status_code, &data_len))
		return false;
	if (status_code != 200)
		return false;
	WDT_Reset();
	if (!sim800_send_at_and_wait_for_response(sim800, strcpy_P(str, PSTR("AT+HTTPREAD\r")), response, max_response_size, 2000))
		return false;		
	if (strstr(response, strcpy_P(str, PSTR("+HTTPREAD:"))) == NULL)
		return false;
	if (!sim800_wait_for_data(sim800, response, data_len, 2000))
		return false;
	response[data_len] = 0;
#ifdef DEBUG_MODE
	comm_println(&DEBUG_USART, response);
#endif*/
	return true;
}
//------------------------------------------------------------------------------
bool sim800_http_init(t_sim800 *sim800){
	bool ok, error, timeout;
	char response[32];
	if (!sim_core_send_at_and_wait_for_response(&sim800->core, "AT+HTTPINIT\r", response, 1000, &ok, &error, &timeout) || !ok || error || timeout)
		return false;
	return true;
}
//------------------------------------------------------------------------------
bool sim800_http_terminate(t_sim800 *sim800){
	bool ok, error, timeout;
	char response[32];
	if (!sim_core_send_at_and_wait_for_response(&sim800->core, "AT+HTTPTERM\r", response, 1000, &ok, &error, &timeout) || !ok || error || timeout)
		return false;
	return true;
}

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
char *substr(char *dest, char *source, int from, int count){
	char *p = dest;
	while (from < strlen(source) && count){
		*p = source[from++];
		p++;
		count--;
	}
	*p = 0;
	return dest;
}
//------------------------------------------------------------------------------
time_t parse_time(char *s){
	struct tm _time;
	int from = 0;
	char aux[8];
	if (strlen(s) != 18)
		return 0;
//	2020 06 15 05 06 55
	_time.tm_year = strtol(substr(aux, s, from, 4), NULL, 10) - 1900; from += 4;
	_time.tm_mon = strtol(substr(aux, s, from, 2), NULL, 10) - 1; from += 2;
	_time.tm_mday = strtol(substr(aux, s, from, 2), NULL, 10); from += 2;
	_time.tm_hour = strtol(substr(aux, s, from, 2), NULL, 10); from += 2;
	_time.tm_min = strtol(substr(aux, s, from, 2), NULL, 10); from += 2;
	_time.tm_sec = strtol(substr(aux, s, from, 2), NULL, 10); from += 2;
	_time.tm_wday = 0;
	_time.tm_yday = 0;
	_time.tm_isdst = -1;
	return mktime(&_time);
}
//------------------------------------------------------------------------------
void sim800_process_gnss_urc(t_sim800 *sim800, char *urc){
	char token[32];
	int token_number = 0;
	t_sim800_gnss_report gnss_report;
	ESP_LOGI(SIM800_TAG, "sim800_process_gnss_urc(%s)", urc);
	gnss_report.run_status = strtol(utils_token(urc, ',', token_number, token, 31), NULL, 10); token_number++;
	gnss_report.fix_status = strtol(utils_token(urc, ',', token_number, token, 31), NULL, 10); token_number++;
	gnss_report.time = parse_time(utils_token(urc, ',', token_number, token, 31)); token_number++;
	gnss_report.latitude = strtod(utils_token(urc, ',', token_number, token, 31), NULL); token_number++;
	gnss_report.longitude = strtod(utils_token(urc, ',', token_number, token, 31), NULL); token_number++;
	gnss_report.altitude = strtod(utils_token(urc, ',', token_number, token, 31), NULL); token_number++;
	gnss_report.speed = strtod(utils_token(urc, ',', token_number, token, 31), NULL); token_number++;
	gnss_report.course = strtod(utils_token(urc, ',', token_number, token, 31), NULL); token_number++;
	gnss_report.fix_mode = strtol(utils_token(urc, ',', token_number, token, 31), NULL, 10); token_number++;
	token_number++; //reserved1
	gnss_report.hdop = strtod(utils_token(urc, ',', token_number, token, 31), NULL); token_number++;
	gnss_report.pdop = strtod(utils_token(urc, ',', token_number, token, 31), NULL); token_number++;
	gnss_report.vdop = strtod(utils_token(urc, ',', token_number, token, 31), NULL); token_number++;
	token_number++;  //reserved2
	gnss_report.satellites_in_view = strtol(utils_token(urc, ',', token_number, token, 31), NULL, 10); token_number++;
	gnss_report.satellites_used = strtol(utils_token(urc, ',', token_number, token, 31), NULL, 10); token_number++;
	token_number++;  //reserved3
	token_number++;  //C/N0 max (?)
	gnss_report.hpa = strtod(utils_token(urc, ',', token_number, token, 31), NULL); token_number++;
	gnss_report.vpa = strtod(utils_token(urc, ',', token_number, token, 31), NULL); token_number++;

#ifdef DEBUG_SIM800_GNSS
	ESP_LOGI(SIM800_TAG, "gnss_report ->\n"
		"run_status: %i\n"
		"fix_status: %i\n"
		"time: %lu\n"
		"latitude: %f\n"
		"longitude: %f\n"
		"altitude: %f\n"
		"speed: %f\n"
		"course: %f\n"
		"fix_mode: %i\n"
		"hdop: %f\n"
		"pdop: %f\n"
		"vdop: %f\n"
		"satellites_in_view: %i\n"
		"satellites_used: %i\n",
		gnss_report.run_status,
		gnss_report.fix_status,
		gnss_report.time,
		gnss_report.latitude,
		gnss_report.longitude,
		gnss_report.altitude,
		gnss_report.speed,
		gnss_report.course,
		gnss_report.fix_mode,
		gnss_report.hdop,
		gnss_report.pdop,
		gnss_report.vdop,
		gnss_report.satellites_in_view,
		gnss_report.satellites_used
	);
#endif

	if (gnss_report.run_status == 1){
		sim800->last_gnss_report = gnss_report;
		if (gnss_report.fix_status)
			sim800->last_gnss_fix_report = gnss_report;
	}
}
//------------------------------------------------------------------------------
void sim800_process_urc(t_sim800 *sim800, char *urc){
#ifdef DEBUG_SIM800
	ESP_LOGI(SIM800_TAG, "sim800_process_urc()\n%s", urc);
#endif
	char *line = strtok(urc, CRLF);  //primer token!
	while (line){  //mientras queden token's
		if (strstr(line, "+UGNSINF: "))
			sim800_process_gnss_urc(sim800, line + strlen("+UGNSINF: "));
/*		if (strstr(line, "+SAPBR") && strstr(line, "DEACT")){
			continue;
		}
		if (strstr(line, "+CPIN")){
			continue;
		}*/
/*		const char c_urc_12[] = "NORMAL POWER DOWN";
		const char c_urc_13[] = "UNDER-VOLTAGE POWER DOWN";
		const char c_urc_14[] = "UNDER-VOLTAGE WARNNING";
		const char c_urc_15[] = "OVER-VOLTAGE POWER DOWN";
		const char c_urc_16[] = "OVER-VOLTAGE WARNNING";*/
		line = strtok(NULL, CRLF);
	}
}
//------------------------------------------------------------------------------
void sim800_poll(t_sim800 *sim800){
	char urc[1024];
//	sim_core_poll(&sim800->core);
	if (sim_core_urc_received(&sim800->core, urc))
		sim800_process_urc(sim800, urc);
}
//------------------------------------------------------------------------------
void sim800_init(t_sim800 *sim800, uart_port_t uart_num, int32_t frame_timeout){
#ifdef DEBUG_SIM800
	esp_log_level_set(SIM800_TAG, ESP_LOG_INFO);
	ESP_LOGI(SIM800_TAG, "sim800_init()");
#endif
	sim_core_init(&sim800->core, uart_num, c_urc, frame_timeout);
}
//------------------------------------------------------------------------------
