/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#include <stdint.h>
#include <stdio.h>

#include "stdout_USART.h"


/* ==========================  Configuration of the ARM Cortex-M85 Processor and Core Peripherals  =========================== */
 #define __CM85_REV                0x0000U /*!< CM85 Core Revision                                                        */
 #define __NVIC_PRIO_BITS          4       /*!< Number of Bits used for Priority Levels                                   */
 #define __Vendor_SysTickConfig    0       /*!< Set to 1 if different SysTick Config is used                              */
 #define __VTOR_PRESENT            1       /*!< Set to 1 if CPU supports Vector Table Offset Register                     */
 #define __MPU_PRESENT             1       /*!< MPU present                                                               */
 #define __FPU_PRESENT             1       /*!< FPU present                                                               */
 #define __FPU_DP                  0       /*!< Double Precision FPU                                                      */
 #define __DSP_PRESENT             1       /*!< DSP extension present                                                     */
 #define __ICACHE_PRESENT          1       /*!< Instruction Cache present                                                 */
 #define __DCACHE_PRESENT          1       /*!< Data Cache present                                                        */
 #define __SAUREGION_PRESENT       1       /*!< SAU region present                                                        */
 #define __PMU_PRESENT             0       /*!< PMU present                                                               */
 #define __PMU_NUM_EVENTCNT        0       /*!< PMU Event Counters                                                        */

 #include "RTE_Components.h"
 #include CMSIS_device_header
 #include "core_cm85.h"

/* SysTick configuration */
void rt_hw_systick_init(void)
{
#ifdef SOC_SERIES_R9A07G0
    SysTimerInterrupt();
#else
    extern uint32_t SystemCoreClock;
    uint32_t ret = SysTick_Config(SystemCoreClock / 1000);
    printf("ret: %d %x\n", ret, ret);
    //NVIC_SetPriority(SysTick_IRQn, 0xFF);
#endif
}
unsigned long int g_cnt = 0;
/**
 * This is the timer interrupt service routine.
 *
 */
void SysTick_Handlerx(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    g_cnt++;

    rt_tick_increase();

    /* leave interrupt */
    rt_interrupt_leave();
}

void rt_hw_board_init(void)
{
    stdout_init();
    printf("%s:%d\n", __func__, __LINE__);
    rt_hw_systick_init();  
}

void rt_hw_console_output(const char *str)
{
    printf("%s", str);
}
