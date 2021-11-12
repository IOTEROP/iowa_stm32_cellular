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
 * @brief   IOWA specific L462 code
 ******************************************************************************
 * @attention
 *
 ******************************************************************************
 */

#include <cellular_service_task.h>
#include <cellular_control_api.h>
#include "iowa_app.h"
#include "iowa_light_control.h"
#include "iowa_accelerometer.h"
#include <assert.h>
#include <string.h>

#include "plf_iowa_app_config.h"

#ifdef USE_STM32L462E_CELL01

#include "stm32l462e_cell1.h"
#include "stm32l462e_cell1_env_sensors.h"
#include "stm32l462e_cell1_motion_sensors.h"

extern iowa_application_context_t iowaApp;


/* Private function prototypes -----------------------------------------------*/

/**
 * @brief  Callback called when a light control value should be changed
 * @param  id - ID of the object
 * @param  powerOn - Light power
 * @param  dimmer - Dimmer settings
 * @param  colour - A string representing a value in some color space
 * @param  userDataCallback - user data passed to iowa_client_add_light_control_object()
 * @param  contextP
 * @retval IOWA_COAP_NO_ERROR in case of success or an error status.
 */
#if (USE_FULL_IOWA_VERSION == 1)
static iowa_status_t prv_iowaclient_lightControl_cb(iowa_sensor_t id,
		bool powerOn,
		int dimmer,
		char* colour,
		void* userDataCallback,
		iowa_context_t contextP)
{
	Led_TypeDef ledId;

	UNUSED(dimmer);
	UNUSED(colour);
	UNUSED(userDataCallback);
	UNUSED(contextP);

	if (id == iowaApp.ledBlueId)
	{
		ledId = BSP_LED_BLUE;
	}
	else if (id == iowaApp.ledRedId)
	{
		ledId = BSP_LED_RED;
	}
	else if (id == iowaApp.ledGreenId)
	{
		ledId = BSP_LED_GREEN;
	}
	else
	{
		return IOWA_COAP_400_BAD_REQUEST;
	}

	if (powerOn == true)
	{
		BSP_LED_On(ledId);
	}
	else
	{
		BSP_LED_Off(ledId);
	}
	return IOWA_COAP_NO_ERROR;
}
#endif /* USE_FULL_IOWA_VERSION */

void iowasensor_interrupt(void)
{
}

void iowasensor_init(void)
{
	float fValue=0;
#if (USE_FULL_IOWA_VERSION == 1)
	MOTION_SENSOR_Axes_t acceleration;
#endif

	/*Add push button*/
	BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);
	iowa_client_IPSO_add_sensor(iowaApp.iowaContext, IOWA_IPSO_PUSH_BUTTON, (float)0.f, NULL, "UserButton", 0, 0, &iowaApp.pushButtonId);

	/*Add light control*/
	BSP_LED_Init(BSP_LED_GREEN);
	BSP_LED_Init(BSP_LED_RED);
	BSP_LED_Init(BSP_LED_BLUE);

#if (USE_FULL_IOWA_VERSION == 1)
	iowa_client_add_light_control_object(iowaApp.iowaContext, 0, 0, "SVG", prv_iowaclient_lightControl_cb, NULL, &iowaApp.ledGreenId);
	iowa_client_add_light_control_object(iowaApp.iowaContext, 0, 0, "SVG", prv_iowaclient_lightControl_cb, NULL, &iowaApp.ledRedId);
	iowa_client_add_light_control_object(iowaApp.iowaContext, 0, 0, "SVG", prv_iowaclient_lightControl_cb, NULL, &iowaApp.ledBlueId);

	iowa_client_light_control_set_state(iowaApp.iowaContext, iowaApp.ledGreenId, false, 0, "#00FF00");
	iowa_client_light_control_set_state(iowaApp.iowaContext, iowaApp.ledRedId, false, 0, "#FF0000");
	iowa_client_light_control_set_state(iowaApp.iowaContext, iowaApp.ledBlueId, false, 0, "#0000FF");
#endif /* USE_FULL_IOWA_VERSION */

	IOWA_APP_TRACE("Add Temperature sensor.");
	BSP_ENV_SENSOR_Init_Temperature();
	BSP_ENV_SENSOR_Read_Temperature(&fValue);
	iowa_client_IPSO_add_sensor(iowaApp.iowaContext, IOWA_IPSO_TEMPERATURE, fValue, "°C", NULL, 15, 40, &iowaApp.temperatureId);

	IOWA_APP_TRACE("Add Humidity sensor.");
	BSP_ENV_SENSOR_Init_Humidity();
	BSP_ENV_SENSOR_ReadT_Humidity(&fValue);
	iowa_client_IPSO_add_sensor(iowaApp.iowaContext, IOWA_IPSO_HUMIDITY, fValue, "%", NULL, 20, 80, &iowaApp.humidityId);

	IOWA_APP_TRACE("Add Pressure sensor.");
	BSP_ENV_SENSOR_Init_Pressure();
	BSP_ENV_SENSOR_Read_Pressure(&fValue);
	iowa_client_IPSO_add_sensor(iowaApp.iowaContext, IOWA_IPSO_PRESSURE, fValue, "hPa", NULL, 260, 1260, &iowaApp.pressureId);

#if (USE_FULL_IOWA_VERSION == 1)
	IOWA_APP_TRACE("Add Motion sensor.");
	BSP_MOTION_SENSOR_Init_Acc();
	BSP_MOTION_SENSOR_Read_Acc(&acceleration);
	IOWA_APP_TRACE_ARG("BSP_MOTION_SENSOR_InitXXXX_Acc values x=%ld y=%ld z=%ld", acceleration.x, acceleration.y, acceleration.z);
	retval = iowa_client_add_accelerometer_object(iowaApp.iowaContext, IOWA_ACCELEROMETER_3_AXIS, 0.0, 0.0, "m/s²",&iowaApp.accelerometerId);

#endif /* USE_FULL_IOWA_VERSION*/

}

/* @brief  Retrieve the code from the sensors board
 * @retval None
 * */
void iowasensor_real_sensors_data(void const *argument)
{
	UNUSED(argument);

	float fValue;
#if (USE_FULL_IOWA_VERSION == 1)
	MOTION_SENSOR_Axes_t acceleration;
#endif

	/*Update push button value*/
	float buttonValue = 0.f;
	if (BSP_PB_GetState(BUTTON_USER) == GPIO_PIN_RESET)
	{
		buttonValue = 1.f;
	}
	iowa_client_IPSO_update_value(iowaApp.iowaContext, iowaApp.pushButtonId, buttonValue);

	if (iowaApp.temperatureId != 0)
	{
		/*Update Temperature sensor value*/
		BSP_ENV_SENSOR_Read_Temperature(&fValue);
		iowa_client_IPSO_update_value(iowaApp.iowaContext, iowaApp.temperatureId, (float) fValue);
	}

	if (iowaApp.humidityId != 0)
	{
		/* Update Humidity sensor value */
		BSP_ENV_SENSOR_ReadT_Humidity(&fValue);
		iowa_client_IPSO_update_value(iowaApp.iowaContext, iowaApp.humidityId, (float) fValue);
	}

	if (iowaApp.pressureId != 0)
	{
		/*Update Pressure sensor value */
		BSP_ENV_SENSOR_Read_Pressure(&fValue);
		iowa_client_IPSO_update_value(iowaApp.iowaContext, iowaApp.pressureId, (float) fValue);
	}

#if (USE_FULL_IOWA_VERSION == 1)
	if (iowaApp.accelerometerId != 0)
	{
		/*Update Pressure sensor value */
		BSP_MOTION_SENSOR_Read_Acc(&acceleration);
		iowa_client_accelerometer_update_axis(iowaApp.iowaContext, iowaApp.accelerometerId, acceleration.x, acceleration.y, acceleration.z);
	}

#endif /* USE_FULL_IOWA_VERSION*/


	/* Debug / Trace information */
	/*
	IOWA_APP_TRACE_ARG("It stay %ld bytes in iowa client task.", uxTaskGetStackHighWaterMark(iowaApp.iowaClientTaskHandle));
	IOWA_APP_TRACE_ARG("It stay %ld bytes in iowa sensor task.", uxTaskGetStackHighWaterMark(iowaApp.iowaSensorTaskHandle));
	IOWA_APP_TRACE_ARG("Current free heap size %d.", xPortGetFreeHeapSize());
	IOWA_APP_TRACE_ARG("Minimum free heap size %d.", xPortGetMinimumEverFreeHeapSize());
	UBaseType_t taskNumber = uxTaskGetNumberOfTasks();
	IOWA_APP_TRACE_ARG("Task number = %ld.", taskNumber);
	TaskStatus_t pxTaskStatusArray[15];
	uint32_t pulTotalRunTime;
	UBaseType_t result = uxTaskGetSystemState( pxTaskStatusArray, taskNumber, &pulTotalRunTime);
	IOWA_APP_TRACE_ARG("True Task number = %ld.", result);
	for (UBaseType_t i = 0; i < result; i++)
	{
	 	IOWA_APP_TRACE_ARG("Task name = %s.", pxTaskStatusArray[i].pcTaskName);
		IOWA_APP_TRACE_ARG("Task state = %d.", pxTaskStatusArray[i].eCurrentState);
		IOWA_APP_TRACE_ARG("Minimum free size %d bytes.", pxTaskStatusArray[i].usStackHighWaterMark);
	}
	 */

	/* IOWA_APP_TRACE_ARG("Max IOWA allocation %d bytes.", iowaApp.maxAllocatedSize);
	IOWA_APP_TRACE_ARG("Current IOWA allocation %d bytes.", iowaApp.currentAllocatedSize);
	 */
}

#endif /* USE_STM32L462E_CELL01 */
