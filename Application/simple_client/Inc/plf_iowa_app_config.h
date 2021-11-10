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
 * @file    plf_iowa_app_config.h
 * @author  IoTerop Team
 * @brief   IOWA application configuration
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef PLF_IOWA_APP_CONFIG_H
#define PLF_IOWA_APP_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/* ========================================== */
/* BEGIN -  IOWASample App specific defines   */
/* ========================================== */

/* Should be updated for specific cellular configuration */
#define USE_CUSTOM_APN							(0)		/* APN customization needed */

/* if FULL_IOWA_VERSION is set then the sample code will use all Iowa(c) features
 * WARNING: This flag should be set manually  */
#define USE_FULL_IOWA_VERSION					(0)   /* 0: not activated, 1: activated */

/* if USE_IOWA_SENSOR_THREAD is set then a specific thread update a sample object */
#define USE_IOWA_SENSOR_THREAD					(1)   /* 0: not activated, 1: activated */

#define IOWA_CLIENT_POWER_UP_LOG 				(1)  /* disable power up log if commented */

/* This warning will alert on wrong configuration */
#if (USE_FULL_IOWA_VERSION == 0) && (defined IOWA_THREAD_SUPPORT)
	#error ("IOWA_THREAD_SUPPORT is only available in the paid version")
#endif


/* ========================================== */
/* END   -  IOWASample App specific defines   */
/* ========================================== */

/* Exported types ------------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#if (USE_CUSTOM_APN == 1)
#define DEFAULT_APN				"GlobalIOT.co"
#define DEFAULT_APN_USER		""
#define DEFAULT_APN_PASSWORD	""
#endif /* USE_CUSTOM_APN */

/* Overwrites X-Cube-Cellular features  */

/* ======================================= */
/* BEGIN -  Miscellaneous functionalities  */
/* ======================================= */

#if !defined SW_DEBUG_VERSION
#define SW_DEBUG_VERSION                         (1U)  /** 0 for SW release version (no traces),
                                                         * 1 for SW debug version */
#endif /* !defined SW_DEBUG_VERSION */
/* Because some traces in CellularApp are using PRINT_FORCE, so next define can not be deactivated */
#define USE_DBG_CHAN_APPLICATION                 (1U)  /* To access to cellular system trace */

/* Active or not the debug trace in CellularApp */
#if (SW_DEBUG_VERSION == 1U)
#define USE_TRACE_APPLICATION                    (1U)  /* 1: CellularApp trace activated   */
#else  /* SW_DEBUG_VERSION == 0U */
#define USE_TRACE_APPLICATION                    (0U)  /* 0: CellularApp trace deactivated */
#endif /* SW_DEBUG_VERSION == 1U */

/* ======================================= */
/* END   -  Miscellaneous functionalities  */
/* ======================================= */

#ifdef __cplusplus
}
#endif

#endif /* PLF_IOWA_APP_CONFIG_H */

/****************************END OF FILE****/