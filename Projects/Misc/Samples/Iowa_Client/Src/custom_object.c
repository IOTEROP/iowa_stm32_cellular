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
* This is the implementation of a simple custom LwM2M Object. This custom 
* object behaves as a very basic IPSO Light Object (as the IPSO Object is only 
* available on the full IOWA version)

******************************************************************************
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef USE_STM32L462E_CELL01
#include "stm32l462e_cell1.h"
#endif

/* Header file containing the definition of the IPSO custom Object */
#include "custom_object.h"

/**
* @brief Callback on object operations
* 
* @retval iowa_status_t
* 
*/
iowa_status_t custom_object_dataCallback(iowa_dm_operation_t operation,
                                         iowa_lwm2m_data_t *dataP,
                                         size_t numData,
                                         void *userData,
                                         iowa_context_t iowaH)
{
  custom_object_values_t *objectValuesP;
  size_t i;
  
  // Retrieve our Object values
  objectValuesP = (custom_object_values_t *)userData;
  
  for (i = 0; i < numData; i++)
  {
    switch (dataP[i].resourceID)
    {
    case 5850:		/* switch On/Off (Boolean) */
      if (operation == IOWA_DM_READ)
      {
        switch (dataP[i].instanceID) {
        case BSP_LED_GREEN:
          dataP[i].value.asBoolean = objectValuesP->bGreenLed;
          break;
        case BSP_LED_RED:
          dataP[i].value.asBoolean = objectValuesP->bRedLed;
          break;
        case BSP_LED_BLUE:
          dataP[i].value.asBoolean = objectValuesP->bBlueLed;
          break;
        }
        
      } else if (operation == IOWA_DM_WRITE)
      {
        
        if (dataP[i].value.asBoolean ==  true)
          BSP_LED_On((Led_TypeDef)dataP[i].instanceID);
        else
          BSP_LED_Off((Led_TypeDef)dataP[i].instanceID);
        
        /* Save current state */
        switch (dataP[i].instanceID) {
        case BSP_LED_GREEN:
          objectValuesP->bGreenLed = dataP[i].value.asBoolean;
          break;
        case BSP_LED_RED:
          objectValuesP->bRedLed= dataP[i].value.asBoolean;
          break;
        case BSP_LED_BLUE:
          objectValuesP->bBlueLed =dataP[i].value.asBoolean;
          break;
        }
        
        /* Notfy of a changed resource */
        iowa_client_object_resource_changed(iowaH, dataP[i].objectID, dataP[i].instanceID, dataP[i].resourceID);
        
      }
      break;
      
    default:
      // Should not happen
      return IOWA_COAP_404_NOT_FOUND;
    }
  }
  
  return IOWA_COAP_NO_ERROR;
}

/************************ (C) COPYRIGHT IoTerop *****END OF FILE****/
