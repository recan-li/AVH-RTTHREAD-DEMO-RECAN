/*---------------------------------------------------------------------------
 * Copyright (c) 2021 Arm Limited (or its affiliates). All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *      Name:    retarget_stdio.c
 *      Purpose: Retarget stdio to USART
 *
 *---------------------------------------------------------------------------*/

#include <stdio.h>
#include "drv_usart.h"

#if (ARM_AVH_UART_PRINTF_ENABLE)

/**
  Initialize stdio
 
  \return          0 on success, or -1 on error.
*/
int stdio_init (void) 
{
    int32_t status;
    status = ptrUSART->Initialize(NULL);
    if (status != ARM_DRIVER_OK) {
        return (-1);
    }

    status = ptrUSART->PowerControl(ARM_POWER_FULL);
    if (status != ARM_DRIVER_OK) {
        return (-1);
    }

    status = ptrUSART->Control(ARM_USART_MODE_ASYNCHRONOUS |
                             ARM_USART_DATA_BITS_8       |
                             ARM_USART_PARITY_NONE       |
                             ARM_USART_STOP_BITS_1       |
                             ARM_USART_FLOW_CONTROL_NONE,
                             USART_BAUDRATE);
    if (status != ARM_DRIVER_OK) {
        return (-1);
    }

    // 设置USART中断的优先级
    NVIC_SetPriority(UART0RX_IRQn, 0); // 0是最高优先级，数值越大优先级越低

    // 使能USART中断
    NVIC_EnableIRQ(UART0RX_IRQn);

    // 使能接收中断
    status = ptrUSART->Control(ARM_USART_CONTROL_RX, 1);
    if (status != ARM_DRIVER_OK) {
        return (-1);
    }

    // 使能USART中断
    status = ptrUSART->Control(ARM_USART_CONTROL_TX, 1); // 如果需要发送，也使能发送中断
    if (status != ARM_DRIVER_OK) {
        return (-1);
    }

    return (0);
}

/**
  Put a character to the stderr
 
  \param[in]   ch  Character to output
  \return          The character written, or -1 on write error.
*/
int stderr_putchar (int ch) 
{
    uint8_t buf[1];

    buf[0] = ch;
    if (ptrUSART->Send(buf, 1) != ARM_DRIVER_OK) {
        return (-1);
    }
    if (ptrUSART->Send(buf, 1) != ARM_DRIVER_OK) {
        return (-1);
    }
    while (ptrUSART->GetTxCount() != 1);
    return (ch);
}

/**
  Put a character to the stdout

  \param[in]   ch  Character to output
  \return          The character written, or -1 on write error.
*/
int stdout_putchar (int ch) 
{
    uint8_t buf[1];

    buf[0] = ch;
    if (ptrUSART->Send(buf, 1) != ARM_DRIVER_OK) {
        return (-1);
    }
    if (ptrUSART->Send(buf, 1) != ARM_DRIVER_OK) {
        return (-1);
    }
    while (ptrUSART->GetTxCount() != 1);
    return (ch);
}

/**
  Get a character from the stdio
 
  \return     The next character from the input, or -1 on read error.
*/
int stdin_getchar (void) 
{
    uint8_t buf[1];

    if (ptrUSART->Receive(buf, 1) != ARM_DRIVER_OK) {
        return (-1);
    }
    while (ptrUSART->GetRxCount() != 1);
    return (buf[0]);
}

#else

int stdio_init(void)
{
    return 0;
}

int stderr_putchar(int ch)
{
    return 0;
}

int stdout_putchar(int ch)
{
    printf("%c", ch);
    return 0;
}

int stdin_getchar(void)
{
    return getchar();
}

#endif

int stdout_puts(const char *s)
{
    while(*s != '\0') {
        stderr_putchar(*s);
        s++;
    }
    return 0;
}
