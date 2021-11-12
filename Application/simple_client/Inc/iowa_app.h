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
 * @file    iowa_app.h
 * @author  IoTerop Team
 * @brief   IOWA application configuration
 ******************************************************************************
 */

#ifndef IOWA_APP_H
#define IOWA_APP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "iowa.h"
#include "iowa_config.h"
#include "iowa_client.h"
#include "iowa_ipso.h"
#include "trace_interface.h"
#include "cellular_control_api.h"
#include "rtosal.h"


#define IOWA_CLIENT_IDENTITY 				"urn:IMEI:"
#define IOWA_CLIENT_IDENTITY_SIZE			9

#define IOWA_MESSAGE_MAX_SIZE				512U
#define IOWA_MESSAGE_MAX_NUMBER				8U
#define IOWA_MESSAGE_OPERATION_TIMEOUT		1000U /* ms or osWaitForever */

#define IOWA_CLIENT_SERVER_ID 				(1U)
#define IOWA_CLIENT_SERVER_LIFETIME			(20U)
#define DEFAULT_TIME_LOOP					(1U)


#define IOWA_CLIENT_SERVER_SECURITY_MODE	IOWA_SEC_NONE
// #define IOWA_CLIENT_SERVER_SECURITY_MODE	IOWA_SEC_PRE_SHARED_KEY

#if (IOWA_CLIENT_SERVER_SECURITY_MODE != IOWA_SEC_NONE)
	#define IOWA_CLIENT_SERVER_URI				"coaps://18.195.192.63" //iowa-server.ioterop.com
	#define IOWA_CLIENT_PSK_KEY                 "123456"		// PSK OGU
	#define IOWA_CLIENT_PSK_KEY_BASE_64         "SW9UZXJvcA=="
#else
	#define IOWA_CLIENT_SERVER_URI				"coap://iowa-server.ioterop.com"
//	#define IOWA_CLIENT_SERVER_URI				"coap://18.195.192.63"

#endif

typedef struct
{
	iowa_context_t 		iowaContext;

	bool				clientRegistered;
	osThreadId 			task_handle;
	osThreadId 			sensors_task_handle;
	osMessageQId		queueId;
	osMutexId 			mutexId;
	volatile bool 		radioAvailable;

	bool				interruptSelect;

	size_t 				currentAllocatedSize;
	size_t 				maxAllocatedSize;
	uint8_t devIdStr[IOWA_CLIENT_IDENTITY_SIZE+CA_IMEI_SIZE_MAX];

	iowa_sensor_t ledRedId;
	iowa_sensor_t ledGreenId;
	iowa_sensor_t ledBlueId;

	iowa_sensor_t temperatureId;
	iowa_sensor_t humidityId;
	iowa_sensor_t pressureId;
	iowa_sensor_t pushButtonId;
	iowa_sensor_t accelerometerId;
	iowa_sensor_t voltID;		/* Fake Value */

}iowa_application_context_t;

typedef struct
{
	uint8_t 		message[IOWA_MESSAGE_MAX_SIZE];
	size_t 			messageLen;
}iowa_queue_t;

typedef enum
{
	IOWA_INIT_FAILURE,
	IOWA_CONFIGURE_FAILURE,
	IOWA_CREATE_THREAD_FAILURE,
	IOWA_DELETE_THREAD_FAILURE,
	IOWA_ADD_FIRMWARE_UPDATE_FAILURE,
	IOWA_ADD_SERVER_FAILURE,
	IOWA_GET_IMEI_FAILURE,
	IOWA_GET_IMSI_FAILURE,
}IOWA_ERROR_ID;

#define PRV_MALLOC 	pvPortMalloc
#define PRV_FREE 	vPortFree

#define IOWA_APP_TRACE(info) TRACE_PRINT(DBG_CHAN_APPLICATION, DBL_LVL_P0, "[IOWA APP] " info "\r\n\r");
#define IOWA_APP_TRACE_ARG(info,arg...) TRACE_PRINT(DBG_CHAN_APPLICATION, DBL_LVL_P0, "[IOWA APP] " info "\r\n\r", ## arg);
#define IOWA_APP_DBG_TRACE(info) TRACE_PRINT(DBG_CHAN_APPLICATION, DBL_LVL_P1, "[IOWA APP] " info "\r\n\r");
#define IOWA_APP_DBG_TRACE_ARG(info,arg...) TRACE_PRINT(DBG_CHAN_APPLICATION, DBL_LVL_P1, "[IOWA APP] " info "\r\n\r", ## arg);
#define IOWA_APP_ERR_TRACE(info) TRACE_PRINT(DBG_CHAN_APPLICATION, DBL_LVL_ERR, "[IOWA APP] " info "\r\n\r");
#define IOWA_APP_ERR_TRACE_ARG(info,arg...) TRACE_PRINT(DBG_CHAN_APPLICATION, DBL_LVL_ERR, "[IOWA APP] " info "\r\n\r", ## arg);

void iowa_client_init(void);
void iowa_client_start(void);

void iowasensor_init(void);
void iowasensor_interrupt(void);
void iowasensor_update_data(void const *argument);

void iowasensor_real_sensors_data(void const *argument);

#ifdef __cplusplus
}
#endif

#endif /* IOWA_APP_H */

/****************************END OF FILE****/
