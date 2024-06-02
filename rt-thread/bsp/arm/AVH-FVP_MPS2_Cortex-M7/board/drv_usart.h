
#ifndef __DRV_USART_H__
#define __DRV_USART_H__

#include "RTE_Components.h"
#include  CMSIS_device_header
//#include "cmsis_os2.h"
#ifdef RTE_Compiler_EventRecorder
#include "EventRecorder.h"
#endif

#include "Driver_USART.h"

#define USART_DRV_NUM           0
#define USART_BAUDRATE          115200

#define _USART_Driver_(n)  Driver_USART##n
#define  USART_Driver_(n) _USART_Driver_(n)
 
extern ARM_DRIVER_USART  USART_Driver_(USART_DRV_NUM);
#define ptrUSART       (&USART_Driver_(USART_DRV_NUM))


#endif


