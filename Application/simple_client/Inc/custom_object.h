/**********************************************
 *
 * Copyright (c) 2016-2020 IoTerop.
 * All rights reserved.
 *
 * This program and the accompanying materials
 * are made available under the terms of
 * IoTerop’s IOWA License (LICENSE.TXT) which
 * accompany this distribution.
 *
 **********************************************/

/********************************************************************************
 *
 * This is the header file for a simple custom
 * LwM2M Object with the following definition:
 *
 * ID: 3311
 * Single instance
 *
 * Resources:
 * |  ID  | Type    | Operations | Multiple | Description                         |
 * +------+---------+------------+----------+-------------------------------------+
 * | 5850 | Boolean |   R/W      | Single   | ON - OFF Control                    |
 *
 *********************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef CUSTOM_OBJECT_H
#define CUSTOM_OBJECT_H

#ifdef __cplusplus
extern "C" {
#endif


/* IOWA headers */
#include "iowa_client.h"

#ifdef USE_STM32L462E_CELL01

#include "stm32l462e_cell1.h"
#include "stm32l462e_cell1_env_sensors.h"
#endif

#define CUSTOM_LED_OBJECT_ID 3311

/* The Custom Object Resources description */
#define CUSTOM_LED_RES_COUNT 1

#define CUSTOM_LED_RES_DESCRIPTION {                                                                          \
    {5850, IOWA_LWM2M_TYPE_BOOLEAN, IOWA_OPERATION_READ | IOWA_OPERATION_WRITE, IOWA_RESOURCE_FLAG_NONE},     \
}

/* A structure containing the values of the writable resources of the object */
typedef struct
{
    bool  bRedLed;
    bool  bGreenLed;
    bool  bBlueLed;

} custom_object_values_t;

/* The function called by IOWA when an operation is to be performed on the Object resources */
iowa_status_t custom_object_dataCallback(iowa_dm_operation_t operation,
                                         iowa_lwm2m_data_t *dataP,
                                         size_t numData,
                                         void *userData,
                                         iowa_context_t iowaH);

#ifdef __cplusplus
}
#endif

#endif /* CUSTOM_OBJECT_H */
/****************************END OF FILE****/