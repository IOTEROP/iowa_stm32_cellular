/**********************************************
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
* Copyright (c) 2016-2020 IoTerop.
* All rights reserved.
*
* This program and the accompanying materials
* are made available under the terms of
* IoTerop’s IOWA License (LICENSE.TXT) which
* accompany this distribution.
*
* In this file, you can define the compilation
* flags instead of specifying them on the
* compiler command-line.
*
**********************************************/

#ifndef _IOWA_CONFIG_INCLUDE_
#define _IOWA_CONFIG_INCLUDE_

/**********************************************
*
* Platform configuration.
*
**********************************************/


/**********************************************
* To specify the endianness of your platform.
* One and only one must be defined.
*/
// #define LWM2M_BIG_ENDIAN
#define LWM2M_LITTLE_ENDIAN

/************************************************
* To specify the size of the static buffer used
* to received datagram packets.
*/
#define IOWA_BUFFER_SIZE 512

/**********************************************
*
* IOWA configuration.
*
**********************************************/

/**********************************************
* Support of transports.
*/
#define IOWA_UDP_SUPPORT
// #define IOWA_TCP_SUPPORT
// #define IOWA_LORAWAN_SUPPORT
// #define IOWA_LORAWAN_MINIMAL_SUPPORT
// #define IOWA_SMS_SUPPORT

/***************************************
* Support only one connection at a time.
* Only relevant for LWM2M_CLIENT_MODE.
*/
// #define IOWA_SINGLE_CONNECTION_MODE

/**********************************************
* Support of CoAP Block-Wise Transfer.
*/
// #define IOWA_COAP_BLOCK_SUPPORT
// #define IOWA_COAP_BLOCK_MINIMAL_SUPPORT

/**********************************************
* To choose the security layer to use.
* Choices are:
*   - IOWA_SECURITY_LAYER_NONE (default)
*   - IOWA_SECURITY_LAYER_USER
*   - IOWA_SECURITY_LAYER_MBEDTLS
*   - IOWA_SECURITY_LAYER_MBEDTLS_PSK_ONLY
*   - IOWA_SECURITY_LAYER_TINYDTLS
*/
//#define IOWA_SECURITY_LAYER IOWA_SECURITY_LAYER_MBEDTLS_PSK_ONLY
#define IOWA_SECURITY_LAYER IOWA_SECURITY_LAYER_NONE

/***********************************************
* To enable logs
* By level:
*     - IOWA_LOG_LEVEL_NONE (default)
*     - IOWA_LOG_LEVEL_ERROR
*     - IOWA_LOG_LEVEL_WARNING
*     - IOWA_LOG_LEVEL_INFO
*     - IOWA_LOG_LEVEL_TRACE
*
* and by components:
*     - IOWA_PART_ALL (default)
*     - IOWA_PART_BASE
*     - IOWA_PART_COAP
*     - IOWA_PART_COMM
*     - IOWA_PART_DATA
*     - IOWA_PART_LWM2M
*     - IOWA_PART_OBJECT
*     - IOWA_PART_SECURITY
*     - IOWA_PART_SYSTEM
*/
#define IOWA_LOG_LEVEL IOWA_LOG_LEVEL_ERROR
#define IOWA_LOG_PART IOWA_PART_ALL

/**********************************************
* To use IOWA in a multi threaded environment.
* The following abstraction functions must be implemented
*   - iowa_system_connection_interrupt_select()
*   - iowa_system_mutex_lock()
*   - iowa_system_mutex_unlock()
*/
//#define IOWA_THREAD_SUPPORT

/**********************************************
* To enable context saving and loading.
* The following abstraction functions must be implemented
*   - iowa_system_store_context()
*   - iowa_system_retrieve_context()
*/
//#define IOWA_STORAGE_CONTEXT_SUPPORT

/**************************************************
* To enable automatic context saving after bootstrap.
* LWM2M_BOOTSTRAP and IOWA_STORAGE_CONTEXT_SUPPORT
* must be defined.
*/
//#define IOWA_STORAGE_CONTEXT_AUTOMATIC_BACKUP

/**********************************************
* To disable system functions check.
*/
//#define IOWA_CONFIG_SKIP_SYSTEM_FUNCTION_CHECK

/**********************************************
* To disable function arguments check.
*/
#define IOWA_CONFIG_SKIP_ARGS_CHECK

/**********************************************
* To implement the Logger's functions.
* If not defined, use the IOWA Logger implementation.
*/
// #define IOWA_LOGGER_USER

/**********************************************
* To enable LWM2M features.
**********************************************/

/*************************************************
* To specify the LWM2M role of your device.
* Several of them can be defined at the same time.
*/
#define LWM2M_CLIENT_MODE
// #define LWM2M_SERVER_MODE
// #define LWM2M_BOOTSTRAP_SERVER_MODE

/**************************************
* Support of the bootstrap interface.
* Only relevant for LWM2M_CLIENT_MODE.
*/
// #define LWM2M_BOOTSTRAP

/**************************************
* To specify version of LwM2M stack
* By default the LwM2M version is 1.0
*/
//#define LWM2M_VERSION_1_0_REMOVE
//#define LWM2M_VERSION_1_1_SUPPORT

/*************************************
* Support of incoming connections.
* Only relevant for LWM2M_CLIENT_MODE.
*/
// #define LWM2M_CLIENT_INCOMING_CONNECTION_SUPPORT

/***************************************
* Support only one server configuration at a time.
* Only relevant for LWM2M_CLIENT_MODE.
*/
#define LWM2M_SINGLE_SERVER_MODE

/**********************************************
* To specify the supported content format.
* Several of them can be defined at the same time.
* Implicit for LWM2M_SERVER_MODE and LWM2M_BOOTSTRAP_SERVER_MODE.
* LWM2M_SUPPORT_TLV is implicit if LWM2M_VERSION_1_0_REMOVE is not set
*/
// #define LWM2M_SUPPORT_JSON
// #define LWM2M_SUPPORT_SENML_JSON
// #define LWM2M_SUPPORT_CBOR
#ifdef LWM2M_VERSION_1_1_SUPPORT
#define LWM2M_SUPPORT_SENML_CBOR
#endif
// #define LWM2M_SUPPORT_TLV

/**************************************************
* To enable storage of notifications on the system
* when the server is not reachable.
*
* The following abstraction functions must be implemented
*   - iowa_system_queue_create()
*   - iowa_system_queue_enqueue()
*   - iowa_system_queue_dequeue()
*   - iowa_system_queue_delete()
*/
// #define LWM2M_STORAGE_QUEUE_SUPPORT

/**********************************************
* Support of old LwM2M content format numbers.
*/
// #define LWM2M_OLD_CONTENT_FORMAT_SUPPORT

/**********************************************
* Maximum stored notification values when not
* able to sent them (per observation).
* Only relevant for LWM2M_CLIENT_MODE.
*/
// #define LWM2M_NOTIFICATION_QUEUE_SIZE 4

/**********************************************
* To add the support of the timestamp.
*/
// #define LWM2M_SUPPORT_TIMESTAMP

/**********************************************
* To add the support of the alternate path.
*/
// #define LWM2M_ALTPATH_SUPPORT

/**********************************************
* To specify the supported composite operations.
* Several of them can be defined at the same time.
* Implicit for LWM2M_SERVER_MODE when LWM2M_VERSION_1_1_SUPPORT is set.
*/
#ifdef LWM2M_VERSION_1_1_SUPPORT
#define LWM2M_READ_COMPOSITE_SUPPORT
#define LWM2M_OBSERVE_COMPOSITE_SUPPORT
#define LWM2M_WRITE_COMPOSITE_SUPPORT
#endif
/**********************************************
* To add the support of data push operation.
*/
// #define LWM2M_DATA_PUSH_SUPPORT

/**********************************************
* To reduce the code size by removing LwM2M features.
*/
// #define LWM2M_READ_OPERATION_REMOVE
// #define LWM2M_DISCOVER_OPERATION_REMOVE
// #define LWM2M_WRITE_OPERATION_REMOVE
// #define LWM2M_WRITE_ATTRIBUTES_OPERATION_REMOVE
// #define LWM2M_EXECUTE_OPERATION_REMOVE
// #define LWM2M_CREATE_OPERATION_REMOVE
// #define LWM2M_DELETE_OPERATION_REMOVE
// #define LWM2M_OBSERVE_OPERATION_REMOVE
// #define LWM2M_OPAQUE_CONTENT_FORMAT_REMOVE
// #define LWM2M_TEXT_CONTENT_FORMAT_REMOVE

/**********************************************
* To enable / disable objects features.
**********************************************/

/***********************************************
* Support of OMA Additional objects.
* Only relevant for LWM2M_CLIENT_MODE.
*/
// #define IOWA_SUPPORT_FIRMWARE_UPDATE_OBJECT
// #define IOWA_SUPPORT_SOFTWARE_COMPONENT_OBJECT
// #define IOWA_SUPPORT_SOFTWARE_MANAGEMENT_OBJECT

/**********************************************
* Maximum time in second to wait between blocks
* during a Firmware update using the push method.
*/

// #define IOWA_FIRMWARE_UPDATE_MAX_BLOCK_INTERVAL 120

/**********************************************
* To disable resources on APN Connection Profile Object.
* Only relevant for LWM2M_CLIENT_MODE.
*/
// #define IOWA_APN_CONNECTION_PROFILE_RSC_APN_REMOVE
// #define IOWA_APN_CONNECTION_PROFILE_RSC_AUTO_SELECT_APN_DEVICE_REMOVE
// #define IOWA_APN_CONNECTION_PROFILE_RSC_ENABLE_STATUS_REMOVE
// #define IOWA_APN_CONNECTION_PROFILE_RSC_USER_NAME_REMOVE
// #define IOWA_APN_CONNECTION_PROFILE_RSC_SECRET_REMOVE
// #define IOWA_APN_CONNECTION_PROFILE_RSC_RECONNECT_SCHEDULE_REMOVE
// #define IOWA_APN_CONNECTION_PROFILE_RSC_VALIDITY_REMOVE
// #define IOWA_APN_CONNECTION_PROFILE_RSC_CONN_ESTABLISHMENT_TIME_REMOVE
// #define IOWA_APN_CONNECTION_PROFILE_RSC_CONN_ESTABLISHMENT_RESULT_REMOVE
// #define IOWA_APN_CONNECTION_PROFILE_RSC_CONN_ESTABLISHMENT_REJECT_CAUSE_REMOVE
// #define IOWA_APN_CONNECTION_PROFILE_RSC_CONNECTION_END_TIME_REMOVE
// #define IOWA_APN_CONNECTION_PROFILE_RSC_TOTAL_BYTES_SENT_REMOVE
// #define IOWA_APN_CONNECTION_PROFILE_RSC_TOTAL_BYTES_RECEIVED_REMOVE
// #define IOWA_APN_CONNECTION_PROFILE_RSC_IP_ADDRESS_REMOVE
// #define IOWA_APN_CONNECTION_PROFILE_RSC_PREFIX_LENGTH_REMOVE
// #define IOWA_APN_CONNECTION_PROFILE_RSC_SUBNET_MASK_REMOVE
// #define IOWA_APN_CONNECTION_PROFILE_RSC_GATEWAY_REMOVE
// #define IOWA_APN_CONNECTION_PROFILE_RSC_PRIMARY_DNS_ADDRESS_REMOVE
// #define IOWA_APN_CONNECTION_PROFILE_RSC_SECONDARY_DNS_ADDRESS_REMOVE
// #define IOWA_APN_CONNECTION_PROFILE_RSC_QCI_REMOVE
// #define IOWA_APN_CONNECTION_PROFILE_RSC_TOTAL_PACKETS_SENT_REMOVE
// #define IOWA_APN_CONNECTION_PROFILE_RSC_PDN_TYPE_REMOVE
// #define IOWA_APN_CONNECTION_PROFILE_RSC_APN_RATE_CONTROL_REMOVE

/**********************************************
* To disable resources on Bearer Selection Object.
* Only relevant for LWM2M_CLIENT_MODE.
* Note: Bearer Selection Object must keep at least one of the next resources.
*/
// #define IOWA_BEARER_SELECTION_RSC_PREFERRED_COMM_BEARER_REMOVE
// #define IOWA_BEARER_SELECTION_RSC_ACCEPTABLE_RSSI_GSM_REMOVE
// #define IOWA_BEARER_SELECTION_RSC_ACCEPTABLE_RSCP_UMTS_REMOVE
// #define IOWA_BEARER_SELECTION_RSC_ACCEPTABLE_RSRP_LTE_REMOVE
// #define IOWA_BEARER_SELECTION_RSC_ACCEPTABLE_RSSI_EV_DO_REMOVE
// #define IOWA_BEARER_SELECTION_RSC_CELL_LOCK_LIST_REMOVE
// #define IOWA_BEARER_SELECTION_RSC_OPERATOR_LIST_REMOVE
// #define IOWA_BEARER_SELECTION_RSC_OPERATOR_LIST_MODE_REMOVE
// #define IOWA_BEARER_SELECTION_RSC_AVAILABLE_PLMNS_REMOVE
// #define IOWA_BEARER_SELECTION_RSC_ACCEPTABLE_RSRP_NB_IOT_REMOVE
// #define IOWA_BEARER_SELECTION_RSC_PLMN_SEARCH_TIMER_REMOVE
// #define IOWA_BEARER_SELECTION_RSC_ATTACH_WO_PDN_CONNECTION_REMOVE

/**********************************************
* To disable resources on Cellular Connectivity Object.
* Only relevant for LWM2M_CLIENT_MODE.
*/
// #define IOWA_CELLULAR_CONNECTIVITY_RSC_SMSC_ADDRESS_REMOVE
// #define IOWA_CELLULAR_CONNECTIVITY_RSC_DISABLE_RADIO_PERIOD_REMOVE
// #define IOWA_CELLULAR_CONNECTIVITY_RSC_MODULE_ACTIVATION_CODE_REMOVE
// #define IOWA_CELLULAR_CONNECTIVITY_RSC_PSM_TIMER_REMOVE
// #define IOWA_CELLULAR_CONNECTIVITY_RSC_ACTIVE_TIMER_REMOVE
// #define IOWA_CELLULAR_CONNECTIVITY_RSC_PLMN_RATE_CONTROL_REMOVE
// #define IOWA_CELLULAR_CONNECTIVITY_RSC_EDRX_PARAM_IU_MODE_REMOVE
// #define IOWA_CELLULAR_CONNECTIVITY_RSC_EDRX_PARAM_WB_S1_MODE_REMOVE
// #define IOWA_CELLULAR_CONNECTIVITY_RSC_EDRX_PARAM_NB_S1_MODE_REMOVE
// #define IOWA_CELLULAR_CONNECTIVITY_RSC_EDRX_PARAM_A_GB_MODE_REMOVE

/**********************************************
* To disable resources on Connectivity Monitoring Object.
* Only relevant for LWM2M_CLIENT_MODE.
*/
// #define IOWA_CONNECTIVITY_MONITORING_RSC_LINK_QUALITY_REMOVE
// #define IOWA_CONNECTIVITY_MONITORING_RSC_ROUTER_IP_ADDR_REMOVE
// #define IOWA_CONNECTIVITY_MONITORING_RSC_LINK_USAGE_REMOVE
// #define IOWA_CONNECTIVITY_MONITORING_RSC_APN_REMOVE
// #define IOWA_CONNECTIVITY_MONITORING_RSC_CELL_ID_REMOVE
// #define IOWA_CONNECTIVITY_MONITORING_RSC_SMNC_REMOVE
// #define IOWA_CONNECTIVITY_MONITORING_RSC_SMCC_REMOVE

/**********************************************
* To disable resources on Device Object.
* Only relevant for LWM2M_CLIENT_MODE.
*/
// #define IOWA_DEVICE_RSC_MANUFACTURER_REMOVE
#define IOWA_DEVICE_RSC_MODEL_NUMBER_REMOVE
#define IOWA_DEVICE_RSC_SERIAL_NUMBER_REMOVE
#define IOWA_DEVICE_RSC_FIRMWARE_VERSION_REMOVE
#define IOWA_DEVICE_RSC_POWER_SOURCE_REMOVE
#define IOWA_DEVICE_RSC_BATTERY_REMOVE
#define IOWA_DEVICE_RSC_CURRENT_TIME_REMOVE
#define IOWA_DEVICE_RSC_UTC_OFFSET_REMOVE
#define IOWA_DEVICE_RSC_TIMEZONE_REMOVE
// #define IOWA_DEVICE_RSC_DEVICE_TYPE_REMOVE
#define IOWA_DEVICE_RSC_HARDWARE_VERSION_REMOVE
// #define IOWA_DEVICE_RSC_SOFTWARE_VERSION_REMOVE

/**********************************************
* To disable resources on Server Object.
* Only relevant for LWM2M_CLIENT_MODE.
*/
// #define IOWA_SERVER_RSC_DISABLE_TIMEOUT_REMOVE
// #define IOWA_SERVER_RSC_DEFAULT_PERIODS_REMOVE
/* Only relevant when LWM2M_VERSION_1_1_SUPPORT is set */
#ifdef LWM2M_VERSION_1_1_SUPPORT
#define IOWA_SERVER_RSC_REGISTRATION_BEHAVIOUR_REMOVE
#define IOWA_SERVER_RSC_COMMUNICATION_ATTEMPTS_REMOVE
#endif
/* Only relevant when LWM2M_BOOTSTRAP is set */
// #define IOWA_SERVER_RSC_BOOTSTRAP_TRIGGER_REMOVE
/* Only relevant when LWM2M_DATA_PUSH_SUPPORT is set */
// #define IOWA_SERVER_RSC_MUTE_SEND_REMOVE

/**********************************************
* To specify the default resources value on Server Object.
* Only relevant for LWM2M_CLIENT_MODE.
*/
// #define IOWA_SERVER_RSC_DISABLE_TIMEOUT_DEFAULT_VALUE 86400
// #define IOWA_SERVER_RSC_STORING_DEFAULT_VALUE         false
// #define IOWA_SERVER_RSC_MUTE_SEND_DEFAULT_VALUE       false


/**********************************************
* To disable resources on Light Control Object.
* Only relevant for LWM2M_CLIENT_MODE.
*/
#define IOWA_LIGHT_CONTROL_RSC_DIMMER_REMOVE
#define IOWA_LIGHT_CONTROL_RSC_ON_TIME_REMOVE
#define IOWA_LIGHT_CONTROL_RSC_CUMULATIVE_ACTIVE_POWER_REMOVE
#define IOWA_LIGHT_CONTROL_RSC_POWER_FACTOR_REMOVE
#define IOWA_LIGHT_CONTROL_RSC_COLOUR_REMOVE


#endif
