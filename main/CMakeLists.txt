idf_component_register(
	SRCS
		"aes.c"
		"MQTTClient.c"
		"MQTTFreeRTOS.c"
		"MQTTPacket.c"
		"MQTTConnectClient.c"
		"MQTTSerializePublish.c"
		"MQTTDeserializePublish.c"
		"sim_core.c"
		"sim7000.c"
		"sim800.c"
		"simple_timer.c"
		"utils.c"
		"widecontrol_v2_main.c"
	INCLUDE_DIRS "."
)