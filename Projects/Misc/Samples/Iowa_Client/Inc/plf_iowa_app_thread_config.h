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
 * @file    plf_cellular_app_thread_config.h
 * @author  IoTerop Team
 * @brief   Threads specific configuration
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef PLF_CELLULAR_APP_THREAD_CONFIG_H
#define PLF_CELLULAR_APP_THREAD_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif


/* Includes ------------------------------------------------------------------*/
#include "plf_features.h"


/* Exported types ------------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/* ======================================= */
/* BEGIN -  CellularApp specific defines   */
/* ======================================= */

/* Main IOWA thread parameters */
#define IOWA_THREAD_NUMBER                 (1U)   /* Number of client created > 0U */
#define IOWA_THREAD_STACK_SIZE             (10096U) /* Thread stack size per instance */
#define IOWA_THREAD_PRIO                   osPriorityNormal

/* Sensors IOWA thread parameters  */
#define IOWA_SENSORS_THREAD_NUMBER                 (1U)   /* Number of client created > 0U */
#define IOWA_SENSORS_THREAD_STACK_SIZE             (1000U) /* Thread stack size per instance */
#define IOWA_SENSORS_THREAD_PRIO                   osPriorityNormal

#define APPLICATION_PARTIAL_HEAP_SIZE     (8196U)

/* Application thread stack size: define the stack size needed by CellularApp */
#define APPLICATION_THREAD_STACK_SIZE            (IOWA_THREAD_STACK_SIZE \
													+ IOWA_SENSORS_THREAD_STACK_SIZE)

/* ======================================= */
/* END   -  CellularApp specific defines   */
/* ======================================= */

#ifdef __cplusplus
}
#endif

#endif /* PLF_IOWA_APP_THREAD_CONFIG_H */

/****************************END OF FILE****/
