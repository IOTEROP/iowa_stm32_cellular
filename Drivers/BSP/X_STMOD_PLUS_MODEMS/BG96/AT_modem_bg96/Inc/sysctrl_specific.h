/**
  ******************************************************************************
  * @file    sysctrl_specific.h
  * @author  MCD Application Team
  * @brief   Header for sysctrl_specific.c module for BG96
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef SYSCTRL_BG96_H
#define SYSCTRL_BG96_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sysctrl.h"

/* Exported constants --------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
sysctrl_status_t SysCtrl_BG96_getDeviceDescriptor(sysctrl_device_type_t type, sysctrl_info_t *p_devices_list);
sysctrl_status_t SysCtrl_BG96_open_channel(sysctrl_device_type_t type);
sysctrl_status_t SysCtrl_BG96_close_channel(sysctrl_device_type_t type);
sysctrl_status_t SysCtrl_BG96_power_on(sysctrl_device_type_t type);
sysctrl_status_t SysCtrl_BG96_power_off(sysctrl_device_type_t type);
sysctrl_status_t SysCtrl_BG96_reset(sysctrl_device_type_t type);
sysctrl_status_t SysCtrl_BG96_sim_select(sysctrl_device_type_t type, sysctrl_sim_slot_t sim_slot);

/* Note: following functions are not part of standard sysctrl API
 * They are very specific to this modem and are called by at_custom files of this modem
 */
sysctrl_status_t SysCtrl_BG96_wakeup_from_PSM(uint32_t delay);

#ifdef __cplusplus
}
#endif

#endif /* SYSCTRL_BG96_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

