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
* This file implements the IOWA system abstraction functions.
* This is tailored for the LwM2M Client.
*
******************************************************************************
*/

/* IOWA header */
#include <stdlib.h>
#include <string.h>

#include "iowa_platform.h"
#include "iowa_app.h"
#include "rtosal.h"

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
  int32_t sockId;
  bool enable;
}iowa_system_connection_t;

/* Global variables ----------------------------------------------------------*/
extern iowa_application_context_t iowaApp;


/* Private functions ---------------------------------------------------------*/

/**
* @brief
* @param
* @retval -
*/
static bool prv_charToInt(char character, uint8_t *integerP)
{
  if ('0' <= character && character <= '9')
  {
    (*integerP) = (uint8_t)(character - '0');
    return true;
  }
  return false;
}

/**
* @brief
* @param
* @retval -
*/
static size_t prv_bufferToInt(uint8_t *buffer,
                              size_t length,
                              uint16_t *dataP)
{
  size_t i;
  uint8_t current;
  
  *dataP = 0;
  
  if (0 == length)
  {
    /* Length is 0 */
    return 0;
  }
  
  i = 0;
  while (i < length)
  {
    if (prv_charToInt((char)buffer[i], &current) == true)
    {
      *dataP *= 10;
      *dataP += (uint16_t)current;
    }
    else
    {
      return i;
    }
    i++;
  }
  return i;
}

/**
* @brief
* @param
* @retval -
*/
static void prv_closeSocket(int32_t sockId)
{
  IOWA_APP_TRACE("Close Socket.");
  (void)com_closesocket(sockId);
}


/**************************************************************
* Abstraction functions to be implemented by the user.
**************************************************************/

/*******************************
* System Abstraction Interface
*
* To be implemented by the user.
*/

/**
* @brief  	Same function as C standard library malloc().
* @param
* @retval -It returns the address of a memory block of size bytes or NULL in case of error.
*/
void * iowa_system_malloc(size_t size)
{
  uint8_t *pointer;
  
  pointer = PRV_MALLOC(sizeof(size_t) + size);
  if (pointer != NULL)
  {
    iowaApp.currentAllocatedSize += size;
    if (iowaApp.currentAllocatedSize > iowaApp.maxAllocatedSize)
    {
      iowaApp.maxAllocatedSize = iowaApp.currentAllocatedSize;
    }
    
    *((size_t *)pointer) = size;
    pointer += sizeof(size_t);
  }
  
  return pointer;
}

/**
* @brief  	Same function as C standard library free().
* 			It releases the memory block at address p.
* @param
* @retval -
*/
void iowa_system_free(void * pointer)
{
  if (pointer != NULL)
  {
    uint8_t *realPointer;
    
    realPointer = (uint8_t *)pointer - sizeof(size_t);
    
    iowaApp.currentAllocatedSize -= *((size_t *)realPointer);
    
    PRV_FREE(realPointer);
  }
}

/**
* @brief 	This function returns the number of seconds elapsed since origin
* 			or a negative value in case of error.
The origin(Epoch, system boot, etc...) does not matter as this
function is used only to determine the elapsed time since the
last call to it.
* @param
* @retval -
*/
int32_t iowa_system_gettime(void)
{
  return (int32_t)(HAL_GetTick()/1000);
}

/**
* @brief 	This function starts a reboot of the system.
* @param
* @retval -
*/
void iowa_system_reboot(void *userData)
{
  NVIC_SystemReset();
}


/*******************************
* Logging Abstraction Interface
*
* To be implemented by the user.
*/

/* This function is called to output the logs when the stack is built with IOWA_WITH_LOGS.
* It can be mapped directly to vprintf().
* Parameters:
* - format: same kind of nil-terminated format string as printf() functions.
* - varArgs: the variable arguments.
* */
void iowa_system_trace(const char * format,
                       va_list varArgs)
{
  char bufferTrace[256] = {0};
  
  if (vsprintf(bufferTrace,format, varArgs) > 0)
  {
    /* Dirty hack to handle end line mess (to be consistent with STM32 traces) -  swap CR LF chars  */
    char *p = strstr(bufferTrace, "\r\n");
    if (p) {
      *p++='\n';
      *p='\r';
    }
    /* End of hack */
    
    traceIF_uartPrintForce((uint8_t)(DBG_CHAN_APPLICATION), (uint8_t *)bufferTrace, (uint16_t)crs_strlen((const uint8_t *)bufferTrace));
  }
}


/*************************************
* Communication Abstraction Interface
*
* To be implemented by the user.
*/

/* This function opens a connection to a host.
* Returned value: a pointer to an user-defined type or NULL in case of error.
* Parameters:
* - type: the type of connection to open.
* - hostname: the hostname to connect to as a nil-terminated string.
* - port: the port to connect to as a nil-terminated string or NULL if irrelevant.
* - userData: the iowa_init() parameter.
* */
void * iowa_system_connection_open(iowa_connection_type_t type,
                                   char * hostname,
                                   char * port,
                                   void * userData)
{
  iowa_system_connection_t *connSystemP = NULL;
  int32_t sockId;
  
  com_sockaddr_in_t address;
  com_sockaddr_t distantaddr;
  uint16_t portInt;
  
  UNUSED(userData);
  
  /* Create socket */
  switch(type)
  {
  case IOWA_CONN_DATAGRAM:
    sockId = com_socket(COM_AF_INET, COM_SOCK_DGRAM, COM_IPPROTO_UDP);
    break;
  case IOWA_CONN_STREAM:
    sockId = com_socket(COM_AF_INET, COM_SOCK_STREAM, COM_IPPROTO_TCP);
    break;
  default:
    return NULL;
  }
  
  IOWA_APP_TRACE_ARG("Create socket with result: %ld.",sockId);
  
  /*Connect socket*/
  if (sockId < 0)
  {
    IOWA_APP_TRACE_ARG("iowa_system_connection_open(): Socket creation failed: %ld.",sockId);
    goto exit_error;
  }
  
  /*Get host ip from name*/
  if (com_gethostbyname((const com_char_t *)hostname, &distantaddr) != COM_SOCKETS_ERR_OK)
  {
    IOWA_APP_TRACE_ARG("Get hostname failed !");
    goto exit_error;
  }
  
  /* Get port */
  if (prv_bufferToInt((uint8_t *)port, strlen(port), &portInt) == 0)
  {
    IOWA_APP_TRACE("Get port failed.");
    goto exit_error;
  }
  
  address.sin_family      = (uint8_t)COM_AF_INET;
  address.sin_port        = COM_HTONS(portInt);
  address.sin_addr.s_addr = ((com_sockaddr_in_t *)&distantaddr)->sin_addr.s_addr;
  
  if (com_connect(sockId,
                  (com_sockaddr_t const *)&address,
                  (int32_t)sizeof(com_sockaddr_in_t))
      != COM_SOCKETS_ERR_OK)
  {
    IOWA_APP_TRACE("connect socket failed.");
    goto exit_error;
  }
  
  connSystemP = (iowa_system_connection_t *)PRV_MALLOC(sizeof(iowa_system_connection_t));
  if (connSystemP == NULL)
  {
    IOWA_APP_TRACE("Allocation failed.");
    goto exit_error;
  }
  
  memset(connSystemP, 0, sizeof(iowa_system_connection_t));
  connSystemP->sockId = sockId;
  connSystemP->enable = true;
  
  
  IOWA_APP_TRACE("Connection open.");
  return connSystemP;
  
exit_error:
  
  prv_closeSocket(sockId);
  return NULL;
}

/* This function sends a buffer on a connection.
* Returned value: the number of bytes sent or a negative number in case of error.
* Parameters:
* - connP: the connection as returned by iowa_system_connection_open().
* - buffer, length: data to send.
* - userData: the iowa_init() parameter.
* */
int iowa_system_connection_send(void * connP,
                                uint8_t * buffer,
                                size_t length,
                                void * userData)
{
  iowa_system_connection_t *connSystemP;
  int32_t result;
  
  UNUSED(userData);
  
  connSystemP = (iowa_system_connection_t *)(connP);
  
  if (connSystemP->enable == false)
  {
    IOWA_APP_TRACE("Socket is disabled.");
    return -1;
  }
  
  /* Send a frame to the specific port to receive the date and time from the remote server */
  result = com_send(connSystemP->sockId,
                    (const com_char_t *)buffer,
                    length,
                    COM_MSG_WAIT);
  IOWA_APP_TRACE_ARG("Send %ld bytes.", result);
  return result;
}

/* This function reads data from a connection in a non-blocking way.
* Returned value: the number of bytes read or a negative number in case of error.
* Parameters:
*  - connP: the connection as returned by iowa_system_connection_open().
*  - buffer: to store the read data.
*  - length: the number of bytes to read.
*  - userData: the iowa_init() parameter.
*/
int iowa_system_connection_recv(void * connP,
                                uint8_t * buffer,
                                size_t length,
                                void * userData)
{
  iowa_application_context_t *contextP;
  iowa_queue_t *queueInfoP;
  int result;
  rtosalStatus status;
  
  UNUSED(connP);
  
  IOWA_APP_TRACE("Entering iowa_system_connection_recv.");
  
  contextP = (iowa_application_context_t *)userData;
  
  status = rtosalMessageQueueGet(contextP->queueId, (uint32_t *) &queueInfoP, IOWA_MESSAGE_OPERATION_TIMEOUT);
  if (status != osEventMessage)	/* TODO check return status */
  {
    IOWA_APP_TRACE_ARG("==> status %d \r\n\r", status);
    result = 0;
  }
  else if (queueInfoP->messageLen == 0
           || queueInfoP->messageLen > length)
  {
    memcpy(buffer, queueInfoP->message, length);
    result = length + 1;
  }
  else
  {
    memcpy(buffer, queueInfoP->message, queueInfoP->messageLen);
    result = queueInfoP->messageLen;
  }
  PRV_FREE(queueInfoP);
  
  
  return result;
}

/*  This function returns an unique identifier for the peer of a connection (e.g. IP address, LoRaWAN DevEUI, SMS MSISDN).
*  Returned value: the length of the identifier or 0 in case of error.
*  Parameters:
*  - connP: the connection as returned by iowa_system_connection_open().
*  - buffer: to store the identifier data.
*  - length: the length of buffer.
*  - userData: the iowa_init() parameter.
*  */
size_t iowa_system_connection_get_peer_identifier(void * connP,
                                                  uint8_t *addrP,
                                                  size_t length,
                                                  void * userData)
{
  iowa_system_connection_t *connSystemP;
  
  UNUSED(userData);
  UNUSED(addrP);
  
  connSystemP = (iowa_system_connection_t *)(connP);
  
  if ((int32_t)sizeof(connSystemP->sockId) > length)
  {
    return 0;
  }
  
  addrP = (uint8_t *)&connSystemP->sockId;
  
  return (int32_t)sizeof(connSystemP->sockId);
}

/*  This functions monitors a list of connections for incoming data during the specified time.
*  Returned value: a positive number if data are available, 0 if the time elapsed or a negative number in case of error.
*  Parameters:
*  - connArray: an array of connections as returned by iowa_system_connection_open().
*  - connCount: The size of the array
*  - timeout: the time to wait for data in seconds.
*  - userData: the iowa_init() parameter.
*/
int iowa_system_connection_select(void ** connArray,
                                  size_t connCount,
                                  int32_t timeout,
                                  void * userData)
{
  iowa_system_connection_t *connSystemP;
  iowa_application_context_t *contextP;
  iowa_queue_t queueInfo = {0};
  int32_t result = 0;
  int32_t time;
  
  IOWA_APP_TRACE_ARG("Entering iowa_system_connection_select with %d connection.", connCount);
  IOWA_APP_TRACE_ARG("timeout is %ld s.", timeout);
  
  /* Only support 1 connection */
  if (connCount > 1)
  {
    IOWA_APP_TRACE_ARG("Only support 1 connection: %d connections.", connCount);
    return -1;
  }
  
  connSystemP = (iowa_system_connection_t *)(*connArray);
  contextP = (iowa_application_context_t *)userData;
  
  if (connCount == 0)
  {
    for (time = 0; time < timeout && contextP->interruptSelect != true; time++)
    {
      /*Wait 1 second*/
      osDelay(1000);
    }
    return 0;
  }
  
  if (connSystemP->enable == false)
  {
    IOWA_APP_TRACE("Socket is disabled.");
    return -1;
  }
  
  for (time = 0; time < timeout && contextP->interruptSelect != true; time++)
  {
    /*Check if a message has been received*/
    result = com_recv(connSystemP->sockId,
                      (com_char_t *)&queueInfo.message,
                      IOWA_MESSAGE_MAX_SIZE,
                      COM_MSG_DONTWAIT);
    
    if (result > 0)
    {
      iowa_queue_t *queueInfoP;
      queueInfo.messageLen = result;
      queueInfoP = (iowa_queue_t *)PRV_MALLOC(sizeof(iowa_queue_t));
      if (queueInfoP == NULL)
      {
        IOWA_APP_TRACE("Queue allocation failed.");
        return -2;
      }
      memcpy(queueInfoP, &queueInfo, sizeof(iowa_queue_t));
      if (rtosalMessageQueuePut(contextP->queueId, (uint32_t)queueInfoP, IOWA_MESSAGE_OPERATION_TIMEOUT)!=osOK)
      {
        IOWA_APP_TRACE("Enqueue message failed.");
        PRV_FREE(queueInfoP);
        return -3;
      }
      break;
    }
    else
    {
      /*Wait 1 second*/
      osDelay(1000);
    }
  }
  
  if (time >= timeout
      || contextP->interruptSelect == true)
  {
    IOWA_APP_TRACE("timeout expired or interrupt select occurred.");
    result = 0;
  }
  contextP->interruptSelect = false;
  IOWA_APP_TRACE_ARG("com_recv result: %ld.", result);
  return result;
}

/*  This functions closes a connection.
*  Returned value: none.
*  Parameters:
*  - connP: the connection as returned by iowa_system_connection_open().
*  - userData: the iowa_init() parameter.
*/
void iowa_system_connection_close(void * connP,
                                  void * userData)
{
  iowa_system_connection_t *connSystemP;
  iowa_application_context_t *contextP;
  rtosalStatus status;
  void * p;
  
  IOWA_APP_TRACE("Connection close.");
  
  connSystemP = (iowa_system_connection_t *)(connP);
  prv_closeSocket(connSystemP->sockId);
  connSystemP->enable = false;
  
  contextP = (iowa_application_context_t *)userData;
  do
  {
    status = rtosalMessageQueueGet(contextP->queueId, (uint32_t *)&p, IOWA_MESSAGE_OPERATION_TIMEOUT);
    if ((status == osOK) || (status == osEventMessage))
    {
      if (p != NULL)
        PRV_FREE(p);
    }
  }while (p != NULL);
}


/*******************************
* Mutex Interface
*
* To be implemented by the user if the define IOWA_THREAD_SUPPORT is used.
*/

/*  This functions interrupts any on-going iowa_system_connection_select().
*  Returned value: none.
*  Parameters:
*  - userData: the iowa_init() parameter.
*/
void iowa_system_connection_interrupt_select(void * userData)
{
  iowa_application_context_t *contextP;
  
  IOWA_APP_TRACE("Interrupt select.");
  
  contextP = (iowa_application_context_t *)userData;
  contextP->interruptSelect = true;
}

/* This function locks a mutex.
*  Returned value: none.
*  Parameters:
*  - userData: the iowa_init() parameter.
* */

void iowa_system_mutex_lock(void * userData)
{
  iowa_application_context_t *contextP;
  
  contextP = (iowa_application_context_t *)userData;
  osMutexWait(contextP->mutexId, osWaitForever);
}

/*  This function releases a mutex.
*  Returned value: none.
*  Parameters:
*  - userData: the iowa_init() parameter.
*  */
void iowa_system_mutex_unlock(void * userData)
{
  iowa_application_context_t *contextP;
  
  contextP = (iowa_application_context_t *)userData;
  osMutexRelease(contextP->mutexId);
}


#if (IOWA_CLIENT_SERVER_SECURITY_MODE != IOWA_SEC_NONE)
/*************************************
* Security Abstraction Interface
*
* To be implemented by the user if the define IOWA_SECURITY_LAYER is not IOWA_SECURITY_LAYER_NONE.
*/

/*  This function returns a random vector of the specified size.
*  Returned value: 0 if the vector has been generated, else if an error occurred.
*  Parameters:
*  - randomBuffer: the generated buffer.
*  - size: the size of the buffer.
*  - userData: the iowa_init() parameter.
*  */
int iowa_system_random_vector_generator(uint8_t *randomBuffer,
                                        size_t size,
                                        void *userData)
{
  size_t i;
  
  for (i = 0; i < size; i++)
  {
    randomBuffer[i] = rand() % 256;
  }
  
  return 0;
}


/* The security data function.
*  Returned value: IOWA_COAP_NO_ERROR in case of success or an error status.
*  Parameters:
*  - peerIdentity: the identity associating to the peer (can be an URI on client side, or the PSK identity on server side, etc).
*  - peerIdentitySize: size of the identity.
*  - securityOp: the security operation.
*  - securityDataP: the security data.
*  - userDataP: the iowa_init() parameter.
*  */
iowa_status_t iowa_system_security_data(const uint8_t *peerIdentity,
                                        size_t peerIdentityLen,
                                        iowa_security_operation_t securityOp,
                                        iowa_security_data_t *securityDataP,
                                        void *userDataP)
{
  switch (securityOp)
  {
  case IOWA_SEC_READ:
    switch (securityDataP->securityMode)
    {
    case IOWA_SEC_PRE_SHARED_KEY:
      if ((peerIdentityLen == strlen(IOWA_CLIENT_SERVER_URI) && memcmp(peerIdentity, IOWA_CLIENT_SERVER_URI, peerIdentityLen) == 0))
      {
        securityDataP->protocol.pskData.identityLen = strlen((char *)iowaApp.devIdStr);
        securityDataP->protocol.pskData.identity = (uint8_t *)iowaApp.devIdStr;
        
        securityDataP->protocol.pskData.privateKeyLen = strlen(IOWA_CLIENT_PSK_KEY);
        securityDataP->protocol.pskData.privateKey = (uint8_t *)IOWA_CLIENT_PSK_KEY;
        
        return IOWA_COAP_NO_ERROR;
      }
      
      /* The peer does not match known Server URIs */
      return IOWA_COAP_404_NOT_FOUND;
      
    default:
      /* Implementation for other modes is not done in this example. */
      break;
    }
    break;
    
  case IOWA_SEC_FREE:
    /* Nothing to do here */
    return IOWA_COAP_NO_ERROR;
    
  default:
    /* Implementation for other operations is not done in this example. */
    break;
  }
  
  return IOWA_COAP_501_NOT_IMPLEMENTED;
}
#endif  /* IOWA_CLIENT_SERVER_SECURITY_MODE */

/************************ (C) COPYRIGHT IoTerop *****END OF FILE****/
