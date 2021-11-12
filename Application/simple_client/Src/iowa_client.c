/*******************************************************************************
 *
 *  _________ _________ ___________ _________
 * |         |         |   |   |   |         |
 * |_________|         |   |   |   |    _    |
 * |         |    |    |   |   |   |         |
 * |         |    |    |           |         |
 * |         |    |    |           |    |    |
 * |         |         |           |    |    |
 * |_________|_________|___________|____|____|
 *
 * Copyright (c) 2019-2021 IoTerop.
 * All rights reserved.
 *
 * This program and the accompanying materials are made available under the terms
 * of IoTerop’s IOWA License (LICENSE.TXT) which accompany this distribution.
 *
 ******************************************************************************
 * @file    iowa_client.c
 * @author  IoTerop Team
 * @brief   IOWA Sample Application
 ******************************************************************************
 * @attention
 *
 ******************************************************************************
 */

#include <stdbool.h>
#include <stdlib.h>

#include "cellular_service_task.h"
#include "cellular_service_datacache.h"

#include "rtosal.h"
#include "dc_common.h"
#include "error_handler.h"

#include "iowa_app.h"
#include "iowa_client.h"
#include "iowa_coap.h"
#include "iowa_ipso.h"

#include "iowa_config.h"

#include "plf_iowa_app_config.h"

/* Header file containing the definition of the custom Object */
#include "custom_object.h"

static osMessageQId cel_events_msg_queue;
iowa_application_context_t iowaApp = {0};

#ifdef IOWA_THREAD_SUPPORT
/**
 * @brief  IOWA Event callback
 * @param  eventP - IOWA event information
 * @param  userDataP - user data passed to iowa_init()
 * @param  contextP
 * @retval -
 */
static void prv_iowaclient_event_cb(iowa_event_t *eventP,
		void *userDataP,
		iowa_context_t contextP)
{
	UNUSED(userDataP);
	UNUSED(contextP);

	switch (eventP->eventType)
	{
	case IOWA_EVENT_REG_REGISTERED:
	{
		iowa_coap_peer_t *peerP;

		iowaApp.clientRegistered = true;
		//iowa_client_set_notification_default_periods(contextP, eventP->serverShortId, 60, 0);
		iowa_client_use_reliable_notifications(contextP, eventP->serverShortId, true);

		peerP = iowa_client_get_server_coap_peer(contextP, eventP->serverShortId);

		if (peerP != NULL)
		{
			uint8_t value;

			value = 11;
			iowa_coap_peer_configuration_set(contextP, peerP, IOWA_COAP_SETTING_ACK_TIMEOUT, &value);
			value = 3;
			iowa_coap_peer_configuration_set(contextP, peerP, IOWA_COAP_SETTING_MAX_RETRANSMIT, &value);
		}
		break;
	}
	case IOWA_EVENT_REG_UNREGISTERED:
		iowaApp.clientRegistered = false;
		break;
	default:;
	}
}
#endif	/* IOWA_THREAD_SUPPORT */

/**
 * @brief  iowa_thread
 * @note   Infinite loop
 * @param  argument - parameter osThread
 * @note   UNUSED
 * @retval -
 */
static void iowa_thread(void const *user_arg) {
	(void) user_arg;
	uint32_t msg_queue;  /* Msg received from the queue      */
	iowa_status_t result;
	iowa_device_info_t info = {0};
#if ( USE_FULL_IOWA_VERSION == 0)
    iowa_lwm2m_resource_desc_t custom_object_resources[CUSTOM_LED_RES_COUNT] = CUSTOM_LED_RES_DESCRIPTION;
    uint16_t instanceIDs[BSP_LED_NB] = {BSP_LED_GREEN, BSP_LED_RED, BSP_LED_BLUE};
    custom_object_values_t objectValues;
#endif

	rtosalMessageQueueGet(cel_events_msg_queue, &msg_queue, RTOSAL_WAIT_FOREVER);

	IOWA_APP_TRACE("Ready");

	info.manufacturer = "STMicroelectronics";
	info.deviceType = "L462E_CELL1";
	info.softwareVersion = "IoTerop_V1-XCC_6.0.1";

	/* Get device Unique ID (using IMEI) */
	memcpy(iowaApp.devIdStr, IOWA_CLIENT_IDENTITY, IOWA_CLIENT_IDENTITY_SIZE);
	if (dc_com_read(&dc_com_db, DC_CELLULAR_INFO, (void *)&cst_cellular_info, sizeof(cst_cellular_info)) == DC_COM_OK)
	{
		char *p = (char *)cst_cellular_info.imei;
		if (*p == ' ') p++;		/* Fix space */
		memcpy(&iowaApp.devIdStr[IOWA_CLIENT_IDENTITY_SIZE], p, CA_IMEI_SIZE_MAX);

	}
	else
	{
		ERROR_Handler(DBG_CHAN_APPLICATION, IOWA_GET_IMEI_FAILURE, ERROR_FATAL);
	}

	IOWA_APP_TRACE_ARG("Device Name %s",iowaApp.devIdStr);

#ifdef IOWA_THREAD_SUPPORT
	if (iowa_client_configure(iowaApp.iowaContext, (const char *)iowaApp.devIdStr, &info, prv_iowaclient_event_cb) != IOWA_COAP_NO_ERROR)
#else
		if (iowa_client_configure(iowaApp.iowaContext, (const char *)iowaApp.devIdStr, &info, NULL) != IOWA_COAP_NO_ERROR)
#endif	/* IOWA_THREAD_SUPPORT */
		{
			ERROR_Handler(DBG_CHAN_APPLICATION, IOWA_CONFIGURE_FAILURE, ERROR_FATAL);
		}

	/* No IPSO object on IOWA evaluation version, so we create a custom object for the LED */
#if ( USE_FULL_IOWA_VERSION == 0)
	objectValues.bRedLed = false;
	objectValues.bGreenLed= false;
	objectValues.bBlueLed = true;

    result = iowa_client_add_custom_object(iowaApp.iowaContext,
    									CUSTOM_LED_OBJECT_ID,                        	/* The ID of our custom object */
                                        3, instanceIDs,                             	/* Three instances object */
                                        CUSTOM_LED_RES_COUNT, custom_object_resources, 	/* the object's resources description */
                                        custom_object_dataCallback,                  /* the callback to handle operations on Resources */
                                        NULL,                                        /* the server can not create new instances */
                                        NULL,                                        /* there are no multiple instances Resources */
                                        &objectValues);                              /* to access our values from the callback */

#endif /* USE_FULL_IOWA_VERSION */

	/* Add Server */
	result = iowa_client_add_server(iowaApp.iowaContext,
			IOWA_CLIENT_SERVER_ID,
			IOWA_CLIENT_SERVER_URI,
			IOWA_CLIENT_SERVER_LIFETIME,
			0,
			IOWA_CLIENT_SERVER_SECURITY_MODE);

	if (result != IOWA_COAP_NO_ERROR)
	{
		ERROR_Handler(DBG_CHAN_APPLICATION, IOWA_ADD_SERVER_FAILURE, ERROR_FATAL);
	}

	iowaApp.clientRegistered = true;

	/* IOWA Loop */
	while(1)
	{
		if (iowaApp.radioAvailable == true)
		{
#ifdef IOWA_THREAD_SUPPORT		/* From IOWA_CONFIG.H */
			result = iowa_step(iowaApp.iowaContext, -1);
#else
			result = iowa_step(iowaApp.iowaContext, DEFAULT_TIME_LOOP);	/* default 1s loop */
#endif
			/* Error management */
			if (result != IOWA_COAP_NO_ERROR)
			{
				iowaApp.clientRegistered = false;
				iowa_client_remove_server(iowaApp.iowaContext, IOWA_CLIENT_SERVER_ID);
				/* Add Server */
				result = iowa_client_add_server(iowaApp.iowaContext,
						IOWA_CLIENT_SERVER_ID,
						IOWA_CLIENT_SERVER_URI,
						IOWA_CLIENT_SERVER_LIFETIME,
						0,
						IOWA_CLIENT_SERVER_SECURITY_MODE);

				if (result == IOWA_COAP_NO_ERROR)
					iowaApp.clientRegistered = true;
			}
		}
		(void)rtosalDelay(500U);
	}
}

#if (USE_IOWA_SENSOR_THREAD == 1)
/**
 * @brief  iowasensor_update_thread
 * @note   Infinite loop IOWA sensor update body
 * @param  argument - parameter osThread
 * @retval -
 */
static void iowasensor_update_thread(void const *user_arg) {
	(void) user_arg;
	iowa_status_t result;
	float volt=0;

	while (iowaApp.clientRegistered == false){
		(void)rtosalDelay(100U);
	}

	/* This voltage sensor is FAKE */
	iowa_client_IPSO_add_sensor(iowaApp.iowaContext, IOWA_IPSO_VOLTAGE, (float)volt, "V", "sample MCU", 0, 125, &iowaApp.voltID);

	for(;;) {
		if (iowaApp.clientRegistered == true) {

			/* Real sensors from board */
			iowasensor_real_sensors_data(user_arg);

			/* Fake sensor code */
			volt = rand() % 255;
			IOWA_APP_TRACE_ARG("===> Update volt %f \r\n\r", volt);
			result = iowa_client_IPSO_update_value(iowaApp.iowaContext, iowaApp.voltID, volt);
			if (result != IOWA_COAP_NO_ERROR) {
				IOWA_APP_TRACE("Error on iowa_client_IPSO_update_value()");
			}
		}
		(void)rtosalDelay(1000U);
	}
}
#endif /* (USE_IOWA_SENSOR_THREAD == 1) */

/* ----------
 * Callback for cellular events
 * - iowaApp.network_up
 */
static void iowaclient_notif_cb(dc_com_event_id_t dc_event_id,
		const void *p_private_gui_data) {
	(void) p_private_gui_data;

	/* Check cellular status */
	if (dc_event_id == DC_CELLULAR_NIFMAN_INFO)
	{
		dc_nifman_info_t dc_nifman_info;
		(void) dc_com_read(&dc_com_db, DC_CELLULAR_NIFMAN_INFO,
				(void *) &dc_nifman_info, sizeof(dc_nifman_info));
		if (dc_nifman_info.rt_state == DC_SERVICE_ON)
		{
			iowaApp.radioAvailable = true;
			iowasensor_interrupt();
			IOWA_APP_TRACE("network is up");
			rtosalMessageQueuePut(cel_events_msg_queue, (uint32_t) dc_event_id, 0);
		} else {
			iowaApp.radioAvailable = false;
			IOWA_APP_TRACE("network is dow");
		}
	}
}


/**
 * @brief  IOWA initialization
 * @param
 * @retval -
 */
void iowa_client_init(void) {

	/* This queue communicates notification from cellular events callback */
	cel_events_msg_queue = rtosalMessageQueueNew(NULL, 1);

	if (!cel_events_msg_queue) {
		IOWA_APP_ERR_TRACE("failed to create Cellular events message queue");
		ERROR_Handler(DBG_CHAN_APPLICATION, 0, ERROR_FATAL);
	}

	/* Register (cb) to datacache */
	dc_com_reg_id_t reg = dc_com_register_gen_event_cb(&dc_com_db, iowaclient_notif_cb, NULL);
	if (reg == DC_COM_INVALID_ENTRY) {
		IOWA_APP_ERR_TRACE("failed to subscribe to datacache events");
		ERROR_Handler(DBG_CHAN_APPLICATION, 0, ERROR_FATAL);
	}

	iowaApp.queueId = rtosalMessageQueueNew(NULL, IOWA_MESSAGE_MAX_NUMBER);
	if (!iowaApp.queueId) {
		IOWA_APP_ERR_TRACE("failed to create IOWA message queue");
		ERROR_Handler(DBG_CHAN_APPLICATION, 0, ERROR_FATAL);
	}

	/* Initialize Iowa */
	iowaApp.iowaContext = iowa_init((void*)&iowaApp);
	if (iowaApp.iowaContext == NULL)
	{
		IOWA_APP_ERR_TRACE("failed to initailize IOWA");
		ERROR_Handler(DBG_CHAN_APPLICATION, IOWA_INIT_FAILURE, ERROR_FATAL);
	}


}

/**
 * @brief  IOWA start - create threads
 * @param
 * @retval -
 */
void iowa_client_start(void) {


	/* Thread Creation */
	iowaApp.task_handle = rtosalThreadNew((const rtosal_char_t *)"Iowa_task",
			(os_pthread)iowa_thread,
			IOWA_THREAD_PRIO, IOWA_THREAD_STACK_SIZE, NULL);

	if (!iowaApp.task_handle) {
		IOWA_APP_ERR_TRACE("failed to create main IOWA thread");
		ERROR_Handler(DBG_CHAN_APPLICATION, IOWA_CREATE_THREAD_FAILURE, ERROR_FATAL);
	}

#if (USE_IOWA_SENSOR_THREAD == 1)

	iowasensor_init();

	iowaApp.sensors_task_handle = rtosalThreadNew((const rtosal_char_t *)"IowaSensorsThread",
			(os_pthread)iowasensor_update_thread,
			IOWA_SENSORS_THREAD_PRIO, IOWA_SENSORS_THREAD_STACK_SIZE, NULL);
	if (!iowaApp.sensors_task_handle)
	{
		IOWA_APP_ERR_TRACE("failed to create IOWA sensors thread");
		ERROR_Handler(DBG_CHAN_APPLICATION, IOWA_CREATE_THREAD_FAILURE, ERROR_FATAL);
	}
#endif /* (USE_IOWA_SENSOR_THREAD == 1) */

}

/************************ (C) COPYRIGHT IoTerop *****END OF FILE****/
