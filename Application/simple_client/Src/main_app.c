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
 * @file    main_app.c
 * @author  IoTerop Team
 * @brief   IOWA Sample Application
 ******************************************************************************
 * @attention
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include <string.h>

#include "FreeRTOS.h"

#include "rtosal.h"
#include "main.h"
#include "task.h"

#include "cellular_control_api.h"
#include "cellular_service_datacache.h"
#include "plf_config.h"

#include "iowa_app.h"

#include "plf_iowa_app_config.h"

#if (USE_CUSTOM_APN == 1)
/**
 * @brief Setup custom APN
 * @retval None
 */
static void setup_custom_apn_modem(void) {
	dc_cellular_params_t params;
	const unsigned int param_size = sizeof(dc_cellular_params_t);
	dc_sim_slot_t *sim_sl;

	/* Read existing values */
	memset(&params, 0, sizeof(params));
	dc_com_read(&dc_com_db, DC_CELLULAR_CONFIG, &params, param_size);

	/* Get sim slot #0 */
	sim_sl = &params.sim_slot[0];

	/* Change the values to custom */
	strcpy((char *) sim_sl->apn, DEFAULT_APN);
	strcpy((char *) sim_sl->username, DEFAULT_APN_USER);
	strcpy((char *) sim_sl->password, DEFAULT_APN_PASSWORD);

	/* Store the new APN */
	dc_com_write(&dc_com_db, DC_CELLULAR_CONFIG, &params, param_size);
}
#endif /* USE_CUSTOM_APN */

/**
 * @brief
 * @retval None
 */
void application_init() {

	/* Initialize cellular software.*/
	cellular_init();

	/* Initialise Iowa stack */
	iowa_client_init();

#if (USE_CUSTOM_APN == 1)
	/* Prepare for custom APN if needed ... */
	setup_custom_apn_modem();
#endif /* USE_CUSTOM_APN */
}

/**
 * @brief
 * @retval None
 */
void application_start() {

	/* Time to connect to cellular  */
	cellular_start();

	/* And run Iowa */
	iowa_client_start();
}

/************************ (C) COPYRIGHT IoTerop *****END OF FILE****/
