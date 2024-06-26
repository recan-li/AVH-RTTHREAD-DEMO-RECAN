/*
 * Copyright (c) 2013-2020 Arm Limited. All rights reserved.
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
 */

#include "Driver_USART.h"

#if 1

#include <stdio.h>

#define ARM_USART_DRV_VERSION    ARM_DRIVER_VERSION_MAJOR_MINOR(1, 0)  /* driver version */

/* Driver Version */
static const ARM_DRIVER_VERSION DriverVersion = { 
    ARM_USART_API_VERSION,
    ARM_USART_DRV_VERSION
};

/* Driver Capabilities */
static const ARM_USART_CAPABILITIES DriverCapabilities = {
    1, /* supports UART (Asynchronous) mode */
    0, /* supports Synchronous Master mode */
    0, /* supports Synchronous Slave mode */
    0, /* supports UART Single-wire mode */
    0, /* supports UART IrDA mode */
    0, /* supports UART Smart Card mode */
    0, /* Smart Card Clock generator available */
    0, /* RTS Flow Control available */
    0, /* CTS Flow Control available */
    0, /* Transmit completed event: \ref ARM_USART_EVENT_TX_COMPLETE */
    0, /* Signal receive character timeout event: \ref ARM_USART_EVENT_RX_TIMEOUT */
    0, /* RTS Line: 0=not available, 1=available */
    0, /* CTS Line: 0=not available, 1=available */
    0, /* DTR Line: 0=not available, 1=available */
    0, /* DSR Line: 0=not available, 1=available */
    0, /* DCD Line: 0=not available, 1=available */
    0, /* RI Line: 0=not available, 1=available */
    0, /* Signal CTS change event: \ref ARM_USART_EVENT_CTS */
    0, /* Signal DSR change event: \ref ARM_USART_EVENT_DSR */
    0, /* Signal DCD change event: \ref ARM_USART_EVENT_DCD */
    0, /* Signal RI change event: \ref ARM_USART_EVENT_RI */
    0  /* Reserved (must be zero) */
};

//
//   Functions
//

static ARM_DRIVER_VERSION ARM_USART_GetVersion(void)
{
  return DriverVersion;
}

static ARM_USART_CAPABILITIES ARM_USART_GetCapabilities(void)
{
  return DriverCapabilities;
}

static int32_t ARM_USART_Initialize(ARM_USART_SignalEvent_t cb_event)
{
    return ARM_DRIVER_OK;
}

static int32_t ARM_USART_Uninitialize(void)
{
    return ARM_DRIVER_OK;
}

static int32_t ARM_USART_PowerControl(ARM_POWER_STATE state)
{
    switch (state)
    {
    case ARM_POWER_OFF:
        break;

    case ARM_POWER_LOW:
        break;

    case ARM_POWER_FULL:
        break;
    }
    return ARM_DRIVER_OK;
}

static int32_t ARM_USART_Send(const void *data, uint32_t num)
{
    if (num == 1) {
        putchar(*(uint8_t *)data);
    }
    return num;
}

static int32_t ARM_USART_Receive(void *data, uint32_t num)
{
    return num;
}

static int32_t ARM_USART_Transfer(const void *data_out, void *data_in, uint32_t num)
{
    return num;
}

static uint32_t ARM_USART_GetTxCount(void)
{
    return ARM_DRIVER_OK;
}

static uint32_t ARM_USART_GetRxCount(void)
{
    return ARM_DRIVER_OK;
}

static int32_t ARM_USART_Control(uint32_t control, uint32_t arg)
{
    return ARM_DRIVER_OK;
}

static ARM_USART_STATUS ARM_USART_GetStatus(void)
{
    ARM_USART_STATUS status = {0};
    return status;
}

static int32_t ARM_USART_SetModemControl(ARM_USART_MODEM_CONTROL control)
{
    return ARM_DRIVER_OK;
}

static ARM_USART_MODEM_STATUS ARM_USART_GetModemStatus(void)
{
    ARM_USART_MODEM_STATUS status = {0};
    return status;
}

static void ARM_USART_SignalEvent(uint32_t event)
{
    // function body
}

// End USART Interface

extern \
ARM_DRIVER_USART Driver_USART0;
ARM_DRIVER_USART Driver_USART0 = {
    ARM_USART_GetVersion,
    ARM_USART_GetCapabilities,
    ARM_USART_Initialize,
    ARM_USART_Uninitialize,
    ARM_USART_PowerControl,
    ARM_USART_Send,
    ARM_USART_Receive,
    ARM_USART_Transfer,
    ARM_USART_GetTxCount,
    ARM_USART_GetRxCount,
    ARM_USART_Control,
    ARM_USART_GetStatus,
    ARM_USART_SetModemControl,
    ARM_USART_GetModemStatus
};

#else

#include <string.h>
#include "ARMCM85.h"

#define RTE_USART0 1

extern uint32_t SystemCoreClock;

#ifndef __IM
#define __IM  volatile const    /*! Defines 'read only' structure member permissions */
#endif
#ifndef __OM
#define __OM  volatile          /*! Defines 'write only' structure member permissions */
#endif
#ifndef __IOM
#define __IOM volatile          /*! Defines 'read/write' structure member permissions */
#endif

/* ================================================================================ */
/* ================              Peripheral memory map             ================ */
/* ================================================================================ */

/* Peripheral and SRAM base address */
#define CMSDK_FLASH_BASE        (0x00000000UL)
#define CMSDK_SRAM_BASE         (0x20000000UL)
#define CMSDK_PERIPH_BASE       (0x40000000UL)

#define CMSDK_RAM_BASE          (0x20000000UL)
#define CMSDK_APB_BASE          (0x40000000UL)
#define CMSDK_AHB_BASE          (0x40010000UL)

/* APB peripherals */
#define CMSDK_TIMER0_BASE       (CMSDK_APB_BASE + 0x0000UL)
#define CMSDK_TIMER1_BASE       (CMSDK_APB_BASE + 0x1000UL)
#define CMSDK_DUALTIMER_BASE    (CMSDK_APB_BASE + 0x2000UL)
#define CMSDK_DUALTIMER_1_BASE  (CMSDK_DUALTIMER_BASE)
#define CMSDK_DUALTIMER_2_BASE  (CMSDK_DUALTIMER_BASE + 0x20UL)
#define CMSDK_UART0_BASE        (CMSDK_APB_BASE + 0x4000UL)
#define CMSDK_UART1_BASE        (CMSDK_APB_BASE + 0x5000UL)
#define CMSDK_UART2_BASE        (CMSDK_APB_BASE + 0x6000UL)
#define CMSDK_WATCHDOG_BASE     (CMSDK_APB_BASE + 0x8000UL)

/* AHB peripherals */
#define CMSDK_GPIO0_BASE        (CMSDK_AHB_BASE + 0x0000UL)
#define CMSDK_GPIO1_BASE        (CMSDK_AHB_BASE + 0x1000UL)
#define CMSDK_SYSCTRL_BASE      (CMSDK_AHB_BASE + 0xF000UL)


/* ================================================================================ */
/* ================             Peripheral declaration             ================ */
/* ================================================================================ */

#define CMSDK_UART0             ((CMSDK_UART_TypeDef              *) CMSDK_UART0_BASE       )
#define CMSDK_UART1             ((CMSDK_UART_TypeDef              *) CMSDK_UART1_BASE       )
#define CMSDK_UART2             ((CMSDK_UART_TypeDef              *) CMSDK_UART2_BASE       )
#define CMSDK_TIMER0            ((CMSDK_TIMER_TypeDef             *) CMSDK_TIMER0_BASE      )
#define CMSDK_TIMER1            ((CMSDK_TIMER_TypeDef             *) CMSDK_TIMER1_BASE      )
#define CMSDK_DUALTIMER         ((CMSDK_DUALTIMER_BOTH_TypeDef    *) CMSDK_DUALTIMER_BASE   )
#define CMSDK_DUALTIMER1        ((CMSDK_DUALTIMER_SINGLE_TypeDef  *) CMSDK_DUALTIMER_1_BASE )
#define CMSDK_DUALTIMER2        ((CMSDK_DUALTIMER_SINGLE_TypeDef  *) CMSDK_DUALTIMER_2_BASE )
#define CMSDK_WATCHDOG          ((CMSDK_WATCHDOG_TypeDef          *) CMSDK_WATCHDOG_BASE    )
#define CMSDK_GPIO0             ((CMSDK_GPIO_TypeDef              *) CMSDK_GPIO0_BASE       )
#define CMSDK_GPIO1             ((CMSDK_GPIO_TypeDef              *) CMSDK_GPIO1_BASE       )
#define CMSDK_SYSCON            ((CMSDK_SYSCON_TypeDef            *) CMSDK_SYSCTRL_BASE     )


// USART Flags
#define USART_FLAG_INITIALIZED      ((uint8_t)(1U))
#define USART_FLAG_POWERED          ((uint8_t)(1U << 1))
#define USART_FLAG_CONFIGURED       ((uint8_t)(1U << 2))
#define USART_FLAG_TX_ENABLED       ((uint8_t)(1U << 3))
#define USART_FLAG_RX_ENABLED       ((uint8_t)(1U << 4))

/*------------- Universal Asynchronous Receiver Transmitter (UART) -----------*/
typedef struct
{
  __IOM  uint32_t  DATA;                     /* Offset: 0x000 (R/W) Data Register    */
  __IOM  uint32_t  STATE;                    /* Offset: 0x004 (R/W) Status Register  */
  __IOM  uint32_t  CTRL;                     /* Offset: 0x008 (R/W) Control Register */
  union {
    __IM   uint32_t  INTSTATUS;              /* Offset: 0x00C (R/ ) Interrupt Status Register */
    __OM   uint32_t  INTCLEAR;               /* Offset: 0x00C ( /W) Interrupt Clear Register  */
    };
  __IOM  uint32_t  BAUDDIV;                  /* Offset: 0x010 (R/W) Baudrate Divider Register */

} CMSDK_UART_TypeDef;

/* CMSDK_UART DATA Register Definitions */
#define CMSDK_UART_DATA_Pos               0                                                  /* CMSDK_UART_DATA_Pos: DATA Position */
#define CMSDK_UART_DATA_Msk              (0xFFUL /*<< CMSDK_UART_DATA_Pos*/)                 /* CMSDK_UART DATA: DATA Mask */

/* CMSDK_UART STATE Register Definitions */
#define CMSDK_UART_STATE_RXOR_Pos         3                                                  /* CMSDK_UART STATE: RXOR Position */
#define CMSDK_UART_STATE_RXOR_Msk        (0x1UL << CMSDK_UART_STATE_RXOR_Pos)                /* CMSDK_UART STATE: RXOR Mask */

#define CMSDK_UART_STATE_TXOR_Pos         2                                                  /* CMSDK_UART STATE: TXOR Position */
#define CMSDK_UART_STATE_TXOR_Msk        (0x1UL << CMSDK_UART_STATE_TXOR_Pos)                /* CMSDK_UART STATE: TXOR Mask */

#define CMSDK_UART_STATE_RXBF_Pos         1                                                  /* CMSDK_UART STATE: RXBF Position */
#define CMSDK_UART_STATE_RXBF_Msk        (0x1UL << CMSDK_UART_STATE_RXBF_Pos)                /* CMSDK_UART STATE: RXBF Mask */

#define CMSDK_UART_STATE_TXBF_Pos         0                                                  /* CMSDK_UART STATE: TXBF Position */
#define CMSDK_UART_STATE_TXBF_Msk        (0x1UL /*<< CMSDK_UART_STATE_TXBF_Pos*/)            /* CMSDK_UART STATE: TXBF Mask */

/* CMSDK_UART CTRL Register Definitions */
#define CMSDK_UART_CTRL_HSTM_Pos          6                                                  /* CMSDK_UART CTRL: HSTM Position */
#define CMSDK_UART_CTRL_HSTM_Msk         (0x01UL << CMSDK_UART_CTRL_HSTM_Pos)                /* CMSDK_UART CTRL: HSTM Mask */

#define CMSDK_UART_CTRL_RXORIRQEN_Pos     5                                                  /* CMSDK_UART CTRL: RXORIRQEN Position */
#define CMSDK_UART_CTRL_RXORIRQEN_Msk    (0x01UL << CMSDK_UART_CTRL_RXORIRQEN_Pos)           /* CMSDK_UART CTRL: RXORIRQEN Mask */

#define CMSDK_UART_CTRL_TXORIRQEN_Pos     4                                                  /* CMSDK_UART CTRL: TXORIRQEN Position */
#define CMSDK_UART_CTRL_TXORIRQEN_Msk    (0x01UL << CMSDK_UART_CTRL_TXORIRQEN_Pos)           /* CMSDK_UART CTRL: TXORIRQEN Mask */

#define CMSDK_UART_CTRL_RXIRQEN_Pos       3                                                  /* CMSDK_UART CTRL: RXIRQEN Position */
#define CMSDK_UART_CTRL_RXIRQEN_Msk      (0x01UL << CMSDK_UART_CTRL_RXIRQEN_Pos)             /* CMSDK_UART CTRL: RXIRQEN Mask */

#define CMSDK_UART_CTRL_TXIRQEN_Pos       2                                                  /* CMSDK_UART CTRL: TXIRQEN Position */
#define CMSDK_UART_CTRL_TXIRQEN_Msk      (0x01UL << CMSDK_UART_CTRL_TXIRQEN_Pos)             /* CMSDK_UART CTRL: TXIRQEN Mask */

#define CMSDK_UART_CTRL_RXEN_Pos          1                                                  /* CMSDK_UART CTRL: RXEN Position */
#define CMSDK_UART_CTRL_RXEN_Msk         (0x01UL << CMSDK_UART_CTRL_RXEN_Pos)                /* CMSDK_UART CTRL: RXEN Mask */

#define CMSDK_UART_CTRL_TXEN_Pos          0                                                  /* CMSDK_UART CTRL: TXEN Position */
#define CMSDK_UART_CTRL_TXEN_Msk         (0x01UL /*<< CMSDK_UART_CTRL_TXEN_Pos*/)            /* CMSDK_UART CTRL: TXEN Mask */

#define CMSDK_UART_INTSTATUS_RXORIRQ_Pos  3                                                  /* CMSDK_UART CTRL: RXORIRQ Position */
#define CMSDK_UART_CTRL_RXORIRQ_Msk      (0x01UL << CMSDK_UART_INTSTATUS_RXORIRQ_Pos)        /* CMSDK_UART CTRL: RXORIRQ Mask */

#define CMSDK_UART_CTRL_TXORIRQ_Pos       2                                                  /* CMSDK_UART CTRL: TXORIRQ Position */
#define CMSDK_UART_CTRL_TXORIRQ_Msk      (0x01UL << CMSDK_UART_CTRL_TXORIRQ_Pos)             /* CMSDK_UART CTRL: TXORIRQ Mask */

#define CMSDK_UART_CTRL_RXIRQ_Pos         1                                                  /* CMSDK_UART CTRL: RXIRQ Position */
#define CMSDK_UART_CTRL_RXIRQ_Msk        (0x01UL << CMSDK_UART_CTRL_RXIRQ_Pos)               /* CMSDK_UART CTRL: RXIRQ Mask */

#define CMSDK_UART_CTRL_TXIRQ_Pos         0                                                  /* CMSDK_UART CTRL: TXIRQ Position */
#define CMSDK_UART_CTRL_TXIRQ_Msk        (0x01UL /*<< CMSDK_UART_CTRL_TXIRQ_Pos*/)           /* CMSDK_UART CTRL: TXIRQ Mask */

/* CMSDK_UART BAUDDIV Register Definitions */
#define CMSDK_UART_BAUDDIV_Pos            0                                                  /* CMSDK_UART BAUDDIV: BAUDDIV Position */
#define CMSDK_UART_BAUDDIV_Msk           (0xFFFFFUL /*<< CMSDK_UART_BAUDDIV_Pos*/)           /* CMSDK_UART BAUDDIV: BAUDDIV Mask */

/*-------------------- General Purpose Input Output (GPIO) -------------------*/
typedef struct
{
  __IOM  uint32_t  DATA;                     /* Offset: 0x000 (R/W) DATA Register */
  __IOM  uint32_t  DATAOUT;                  /* Offset: 0x004 (R/W) Data Output Latch Register */
         uint32_t  RESERVED0[2];
  __IOM  uint32_t  OUTENSET;                 /* Offset: 0x010 (R/W) Output Enable Set Register */
  __IOM  uint32_t  OUTENCLR;                 /* Offset: 0x014 (R/W) Output Enable Clear Register */
  __IOM  uint32_t  ALTFUNCSET;               /* Offset: 0x018 (R/W) Alternate Function Set Register */
  __IOM  uint32_t  ALTFUNCCLR;               /* Offset: 0x01C (R/W) Alternate Function Clear Register */
  __IOM  uint32_t  INTENSET;                 /* Offset: 0x020 (R/W) Interrupt Enable Set Register */
  __IOM  uint32_t  INTENCLR;                 /* Offset: 0x024 (R/W) Interrupt Enable Clear Register */
  __IOM  uint32_t  INTTYPESET;               /* Offset: 0x028 (R/W) Interrupt Type Set Register */
  __IOM  uint32_t  INTTYPECLR;               /* Offset: 0x02C (R/W) Interrupt Type Clear Register */
  __IOM  uint32_t  INTPOLSET;                /* Offset: 0x030 (R/W) Interrupt Polarity Set Register */
  __IOM  uint32_t  INTPOLCLR;                /* Offset: 0x034 (R/W) Interrupt Polarity Clear Register */
  union {
    __IM   uint32_t  INTSTATUS;              /* Offset: 0x038 (R/ ) Interrupt Status Register */
    __OM   uint32_t  INTCLEAR;               /* Offset: 0x038 ( /W) Interrupt Clear Register */
    };
         uint32_t RESERVED1[241];
  __IOM  uint32_t LB_MASKED[256];            /* Offset: 0x400 - 0x7FC Lower byte Masked Access Register (R/W) */
  __IOM  uint32_t UB_MASKED[256];            /* Offset: 0x800 - 0xBFC Upper byte Masked Access Register (R/W) */
} CMSDK_GPIO_TypeDef;

typedef struct _PIN
{
  CMSDK_GPIO_TypeDef *PinGpio;   // Pin GPIO
  uint8_t             PinNum;    // Pin Number
} PIN;

// USART Pin Configuration
typedef const struct _USART_PINS {
        PIN                       *pin_rx;            // RX  Pin identifier
        PIN                       *pin_tx;            // TX  Pin identifier
} USART_PINS;

// USART Transfer Information (Run-Time)
typedef struct _USART_TRANSFER_INFO {
  uint32_t              rx_num;         // Total number of receive data
  uint32_t              tx_num;         // Total number of transmit data
  uint8_t              *rx_buf;         // Pointer to in data buffer
  const uint8_t        *tx_buf;         // Pointer to out data buffer
  uint32_t              rx_cnt;         // Number of data received
  uint32_t              tx_cnt;         // Number of data sent
} USART_TRANSFER_INFO;

typedef struct _USART_STATUS {
  uint8_t tx_busy;                      // Transmitter busy flag
  uint8_t rx_busy;                      // Receiver busy flag
  uint8_t rx_overflow;                  // Receive data overflow detected (cleared on start of next receive operation)
} USART_STATUS;

// USART Information (Run-Time)
typedef struct _USART_INFO {
  ARM_USART_SignalEvent_t cb_event;            // Event callback
  USART_STATUS            status;              // Status flags
  uint8_t                 flags;               // Current USART flags
} USART_INFO;

// USART Resources Definitions
typedef const struct {
  ARM_USART_CAPABILITIES  capabilities;        // Capabilities
  USART_PINS               pins;               // USART pins configuration
  CMSDK_UART_TypeDef      *reg;                // USART peripheral pointer
  IRQn_Type                irq_num_rx;         // USART RX IRQ Number
  IRQn_Type                irq_num_tx;         // USART TX IRQ Number
  USART_INFO              *info;               // Run-Time Information
  USART_TRANSFER_INFO     *xfer;               // USART transfer information
} USART_RESOURCES;


////////////////////////////////////////////


#define UNUSED(x) (void)(x)           /* macro to get rid of 'unused parameter' warning */

#define ARM_USART_DRV_VERSION ARM_DRIVER_VERSION_MAJOR_MINOR(1,0)

// Driver Version
static const ARM_DRIVER_VERSION usart_driver_version = { ARM_USART_API_VERSION, ARM_USART_DRV_VERSION };


// USART0
#if (defined (RTE_USART0) && (RTE_USART0 != 0))

// USART0 Run-Time Information
static USART_INFO          USART0_Info         = { 0U, { 0U, 0U, 0U }, 0U };
static USART_TRANSFER_INFO USART0_TransferInfo = { 0U, 0U, 0U, 0U, 0U, 0U };
static PIN                 USART0_pin_rx       = { CMSDK_GPIO1, 0U };
static PIN                 USART0_pin_tx       = { CMSDK_GPIO1, 1U };


// USART0 Resources
static const USART_RESOURCES USART0_Resources = {
  {     // Capabilities
    1,  // supports UART (Asynchronous) mode
    0,  // supports Synchronous Master mode
    0,  // supports Synchronous Slave mode
    0,  // supports UART Single-wire mode
    0,  // supports UART IrDA mode
    0,  // supports UART Smart Card mode
    0,  // Smart Card Clock generator
    0,  // RTS Flow Control available
    0,  // CTS Flow Control available
    1,  // Transmit completed event: \ref ARM_USART_EVENT_TX_COMPLETE
    1,  // Signal receive character timeout event: \ref ARM_USART_EVENT_RX_TIMEOUT
    0,  // RTS Line: 0=not available, 1=available
    0,  // CTS Line: 0=not available, 1=available
    0,  // DTR Line: 0=not available, 1=available
    0,  // DSR Line: 0=not available, 1=available
    0,  // DCD Line: 0=not available, 1=available
    0,  // RI Line: 0=not available, 1=available
    0,  // Signal CTS change event: \ref ARM_USART_EVENT_CTS
    0,  // Signal DSR change event: \ref ARM_USART_EVENT_DSR
    0,  // Signal DCD change event: \ref ARM_USART_EVENT_DCD
    0,  // Signal RI change event: \ref ARM_USART_EVENT_RI
    0   // Reserved (must be zero)
  },
  {
    &USART0_pin_rx,
    &USART0_pin_tx
  },
  CMSDK_UART0,
  UART0RX_IRQn,
  UART0TX_IRQn,
  &USART0_Info,
  &USART0_TransferInfo
};

#endif

// USART1
#if (defined (RTE_USART1) && (RTE_USART1 != 0))

// USART1 Run-Time Information
static USART_INFO          USART1_Info         = { 0U, { 0U, 0U, 0U }, 0U };
static USART_TRANSFER_INFO USART1_TransferInfo = { 0U, 0U, 0U, 0U, 0U, 0U };
static PIN                 USART1_pin_rx       = { CMSDK_GPIO1, 7U };
static PIN                 USART1_pin_tx       = { CMSDK_GPIO1, 8U };

// USART1 Resources
static const USART_RESOURCES USART1_Resources = {
  {     // Capabilities
    1,  // supports UART (Asynchronous) mode
    0,  // supports Synchronous Master mode
    0,  // supports Synchronous Slave mode
    0,  // supports UART Single-wire mode
    0,  // supports UART IrDA mode
    0,  // supports UART Smart Card mode
    0,  // Smart Card Clock generator
    0,  // RTS Flow Control available
    0,  // CTS Flow Control available
    1,  // Transmit completed event: \ref ARM_USART_EVENT_TX_COMPLETE
    1,  // Signal receive character timeout event: \ref ARM_USART_EVENT_RX_TIMEOUT
    0,  // RTS Line: 0=not available, 1=available
    0,  // CTS Line: 0=not available, 1=available
    0,  // DTR Line: 0=not available, 1=available
    0,  // DSR Line: 0=not available, 1=available
    0,  // DCD Line: 0=not available, 1=available
    0,  // RI Line: 0=not available, 1=available
    0,  // Signal CTS change event: \ref ARM_USART_EVENT_CTS
    0,  // Signal DSR change event: \ref ARM_USART_EVENT_DSR
    0,  // Signal DCD change event: \ref ARM_USART_EVENT_DCD
    0,  // Signal RI change event: \ref ARM_USART_EVENT_RI
    0   // Reserved (must be zero)
  },
  {
    &USART1_pin_rx,
    &USART1_pin_tx
  },
  CMSDK_UART1,
  UART1RX_IRQn,
  UART1TX_IRQn,
  &USART1_Info,
  &USART1_TransferInfo
};
#endif

// USART2
#if (defined (RTE_USART2) && (RTE_USART2 != 0))

// USART2 Run-Time Information
static USART_INFO          USART2_Info         = { 0U, { 0U, 0U, 0U }, 0U };
static USART_TRANSFER_INFO USART2_TransferInfo = { 0U, 0U, 0U, 0U, 0U, 0U };
static PIN                 USART2_pin_rx       = { CMSDK_GPIO1, 0U };
static PIN                 USART2_pin_tx       = { CMSDK_GPIO1, 1U };

// USART2 Resources
static const USART_RESOURCES USART2_Resources = {
  {     // Capabilities
    1,  // supports UART (Asynchronous) mode
    0,  // supports Synchronous Master mode
    0,  // supports Synchronous Slave mode
    0,  // supports UART Single-wire mode
    0,  // supports UART IrDA mode
    0,  // supports UART Smart Card mode
    0,  // Smart Card Clock generator
    0,  // RTS Flow Control available
    0,  // CTS Flow Control available
    1,  // Transmit completed event: \ref ARM_USART_EVENT_TX_COMPLETE
    1,  // Signal receive character timeout event: \ref ARM_USART_EVENT_RX_TIMEOUT
    0,  // RTS Line: 0=not available, 1=available
    0,  // CTS Line: 0=not available, 1=available
    0,  // DTR Line: 0=not available, 1=available
    0,  // DSR Line: 0=not available, 1=available
    0,  // DCD Line: 0=not available, 1=available
    0,  // RI Line: 0=not available, 1=available
    0,  // Signal CTS change event: \ref ARM_USART_EVENT_CTS
    0,  // Signal DSR change event: \ref ARM_USART_EVENT_DSR
    0,  // Signal DCD change event: \ref ARM_USART_EVENT_DCD
    0,  // Signal RI change event: \ref ARM_USART_EVENT_RI
    0   // Reserved (must be zero)
  },
  {
    &USART2_pin_rx,
    &USART2_pin_tx
  },
  CMSDK_UART2,
  UART2RX_IRQn,
  UART2TX_IRQn,
  &USART2_Info,
  &USART2_TransferInfo
};
#endif

// USART3
#if (defined (RTE_USART3) && (RTE_USART3 != 0))

// USART3 Run-Time Information
static USART_INFO          USART3_Info         = { 0U, { 0U, 0U, 0U }, 0U };
static USART_TRANSFER_INFO USART3_TransferInfo = { 0U, 0U, 0U, 0U, 0U, 0U };
static PIN                 USART3_pin_rx       = { CMSDK_GPIO0, 0U };
static PIN                 USART3_pin_tx       = { CMSDK_GPIO0, 4U };

// USART3 Resources
static const USART_RESOURCES USART3_Resources = {
  {     // Capabilities
    1,  // supports UART (Asynchronous) mode
    0,  // supports Synchronous Master mode
    0,  // supports Synchronous Slave mode
    0,  // supports UART Single-wire mode
    0,  // supports UART IrDA mode
    0,  // supports UART Smart Card mode
    0,  // Smart Card Clock generator
    0,  // RTS Flow Control available
    0,  // CTS Flow Control available
    1,  // Transmit completed event: \ref ARM_USART_EVENT_TX_COMPLETE
    1,  // Signal receive character timeout event: \ref ARM_USART_EVENT_RX_TIMEOUT
    0,  // RTS Line: 0=not available, 1=available
    0,  // CTS Line: 0=not available, 1=available
    0,  // DTR Line: 0=not available, 1=available
    0,  // DSR Line: 0=not available, 1=available
    0,  // DCD Line: 0=not available, 1=available
    0,  // RI Line: 0=not available, 1=available
    0,  // Signal CTS change event: \ref ARM_USART_EVENT_CTS
    0,  // Signal DSR change event: \ref ARM_USART_EVENT_DSR
    0,  // Signal DCD change event: \ref ARM_USART_EVENT_DCD
    0,  // Signal RI change event: \ref ARM_USART_EVENT_RI
    0   // Reserved (must be zero)
  },
  {
    &USART3_pin_rx,
    &USART3_pin_tx
  },
  CMSDK_UART3,
  UART3RX_IRQn,
  UART3TX_IRQn,
  &USART3_Info,
  &USART3_TransferInfo
};
#endif


// USART Driver functions

/**
  \fn          ARM_DRIVER_VERSION USARTx_GetVersion (void)
  \brief       Get driver version.
  \return      \ref ARM_DRIVER_VERSION
*/
static ARM_DRIVER_VERSION USARTx_GetVersion (void) {
  return usart_driver_version;
}

/**
  \fn          ARM_USART_CAPABILITIES USART_GetCapabilities (const USART_RESOURCES *usart)
  \brief       Get driver capabilities
  \param[in]   usart     Pointer to USART resources
  \return      \ref ARM_USART_CAPABILITIES
*/
static ARM_USART_CAPABILITIES USART_GetCapabilities (const USART_RESOURCES *usart) {
  return usart->capabilities;
}

/**
  \fn          int32_t USART_Initialize (      ARM_USART_SignalEvent_t  cb_event
                                         const USART_RESOURCES         *usart)
  \brief       Initialize USART Interface.
  \param[in]   cb_event  Pointer to \ref ARM_USART_SignalEvent
  \param[in]   usart     Pointer to USART resources
  \return      \ref execution_status
*/
static int32_t USART_Initialize (      ARM_USART_SignalEvent_t  cb_event,
                                 const USART_RESOURCES         *usart) {

  if (usart->info->flags & USART_FLAG_INITIALIZED) {
    // Driver is already initialized
    return ARM_DRIVER_OK;
  }

  // Initialize callback
  usart->info->cb_event = cb_event;

  // Clear transfer information
  memset(usart->xfer, 0U, sizeof(USART_TRANSFER_INFO));

  // configure USART pins
  usart->pins.pin_rx->PinGpio->ALTFUNCSET |= (1u << usart->pins.pin_rx->PinNum);
  usart->pins.pin_tx->PinGpio->ALTFUNCSET |= (1u << usart->pins.pin_tx->PinNum);

  usart->info->flags = USART_FLAG_INITIALIZED;                  // Set status flags

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t USART_Uninitialize (const USART_RESOURCES *usart)
  \brief       De-initialize USART Interface.
  \param[in]   usart     Pointer to USART resources
  \return      \ref execution_status
*/
static int32_t USART_Uninitialize (const USART_RESOURCES *usart) {

  // unconfigure USART pins
  usart->pins.pin_rx->PinGpio->ALTFUNCSET &= ~(1u << usart->pins.pin_rx->PinNum);
  usart->pins.pin_tx->PinGpio->ALTFUNCSET &= ~(1u << usart->pins.pin_tx->PinNum);

  usart->info->flags = 0U;                                      // Reset status flags

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t USART_PowerControl (ARM_POWER_STATE state)
  \brief       Control USART Interface Power.
  \param[in]   state  Power state
  \param[in]   usart  Pointer to USART resources
  \return      \ref execution_status
*/
static int32_t USART_PowerControl (      ARM_POWER_STATE  state,
                                   const USART_RESOURCES *usart) {

  switch (state) {
    case ARM_POWER_OFF:
      // Clear powered flag
      usart->info->flags &= ~USART_FLAG_POWERED;

      // Clear and disable USART IRQ
      NVIC_DisableIRQ(usart->irq_num_rx);
      NVIC_ClearPendingIRQ(usart->irq_num_rx);
      NVIC_DisableIRQ(usart->irq_num_tx);
      NVIC_ClearPendingIRQ(usart->irq_num_tx);
      break;

    case ARM_POWER_LOW:
      return ARM_DRIVER_ERROR_UNSUPPORTED;

    case ARM_POWER_FULL:
      if ((usart->info->flags & USART_FLAG_INITIALIZED) == 0U) {
        return ARM_DRIVER_ERROR;
      }
      if ((usart->info->flags & USART_FLAG_POWERED)     != 0U) {
        return ARM_DRIVER_OK;
      }

      // Initialize status flags
      usart->info->status.tx_busy       = 0U;
      usart->info->status.rx_busy       = 0U;
      usart->info->status.rx_overflow   = 0U;

      // Set flag initialized
      usart->info->flags = USART_FLAG_POWERED | USART_FLAG_INITIALIZED;

      // Clear and enable USART IRQ
      NVIC_ClearPendingIRQ(usart->irq_num_rx);
      NVIC_EnableIRQ(usart->irq_num_rx);
      NVIC_ClearPendingIRQ(usart->irq_num_tx);
      NVIC_EnableIRQ(usart->irq_num_tx);
      break;

//    default: return ARM_DRIVER_ERROR_UNSUPPORTED;
  }
  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t USART_Send (const void            *data,
                                         uint32_t         num,
                                   const USART_RESOURCES *usart)
  \brief       Start sending data to USART transmitter.
  \param[in]   data  Pointer to buffer with data to send to USART transmitter
  \param[in]   num   Number of data items to send
  \param[in]   usart Pointer to USART resources
  \return      \ref execution_status
*/
static int32_t USART_Send (const void            *data,
                                 uint32_t         num,
                           const USART_RESOURCES *usart) {

  if ((data == NULL) || (num == 0U)) {
    // Invalid parameters
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  if ((usart->info->flags & USART_FLAG_CONFIGURED) == 0U) {
    // USART is not configured (mode not selected)
    return ARM_DRIVER_ERROR;
  }

  if ((usart->info->status.tx_busy != 0U) ||
     ((usart->reg->STATE & CMSDK_UART_STATE_TXBF_Msk) != 0U)) {
    // Send is not completed yet
    return ARM_DRIVER_ERROR_BUSY;
  }

  // Set tx_busy flag
  usart->info->status.tx_busy = 1U;

  // Save transmit buffer info
  usart->xfer->tx_buf = (const uint8_t *)data;
  usart->xfer->tx_num = num;
  usart->xfer->tx_cnt = 0U;

  // TX interrupt enable
  usart->reg->CTRL |= CMSDK_UART_CTRL_TXIRQEN_Msk;

  // Start sending
  usart->reg->DATA = usart->xfer->tx_buf[usart->xfer->tx_cnt];

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t USART_Receive (      void            *data,
                                            uint32_t         num,
                                      const USART_RESOURCES *usart)
  \brief       Start receiving data from USART receiver.
  \param[out]  data  Pointer to buffer for data to receive from USART receiver
  \param[in]   num   Number of data items to receive
  \param[in]   usart Pointer to USART resources
  \return      \ref execution_status
*/
static int32_t USART_Receive (      void            *data,
                                    uint32_t         num,
                              const USART_RESOURCES *usart) {

  if ((data == NULL) || (num == 0U)) {
    // Invalid parameters
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  if ((usart->info->flags & USART_FLAG_CONFIGURED) == 0U) {
    // USART is not configured (mode not selected)
    return ARM_DRIVER_ERROR;
  }

  // Check if receiver is busy
  if ((usart->info->status.rx_busy != 0U) ||
     ((usart->reg->STATE & CMSDK_UART_STATE_RXBF_Msk) != 0U)) {
    return ARM_DRIVER_ERROR_BUSY;
  }

  // Save number of data to be received
  usart->xfer->rx_num = num;

  // Clear RX status
  usart->info->status.rx_overflow = 0U;

  // Save receive buffer info
  usart->xfer->rx_buf = (uint8_t *)data;
  usart->xfer->rx_cnt =  0U;

  // Set RX busy flag
  usart->info->status.rx_busy = 1U;

  // RX interrupt enable
  usart->reg->CTRL |= CMSDK_UART_CTRL_RXIRQEN_Msk;

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t USART_Transfer (const void             *data_out,
                                             void             *data_in,
                                             uint32_t          num,
                                       const USART_RESOURCES  *usart)
  \brief       Start sending/receiving data to/from USART transmitter/receiver.
  \param[in]   data_out  Pointer to buffer with data to send to USART transmitter
  \param[out]  data_in   Pointer to buffer for data to receive from USART receiver
  \param[in]   num       Number of data items to transfer
  \param[in]   usart     Pointer to USART resources
  \return      \ref execution_status
*/
static int32_t USART_Transfer (const void             *data_out,
                                     void             *data_in,
                                     uint32_t          num,
                               const USART_RESOURCES  *usart) {
  UNUSED(data_out);
  UNUSED(data_in);
  UNUSED(num);
  UNUSED(usart);

  // Synchronous mode not supported
  return ARM_DRIVER_ERROR_UNSUPPORTED;
}

/**
  \fn          uint32_t USART_GetTxCount (const USART_RESOURCES *usart)
  \brief       Get transmitted data count.
  \param[in]   usart     Pointer to USART resources
  \return      number of data items transmitted
*/
static uint32_t USART_GetTxCount (const USART_RESOURCES *usart) {

  return usart->xfer->tx_cnt;

}

/**
  \fn          uint32_t USART_GetRxCount (const USART_RESOURCES *usart)
  \brief       Get received data count.
  \param[in]   usart     Pointer to USART resources
  \return      number of data items received
*/
static uint32_t USART_GetRxCount (const USART_RESOURCES *usart) {

  return usart->xfer->rx_cnt;

}

/**
  \fn          int32_t USART_Control (      uint32_t          control,
                                            uint32_t          arg,
                                      const USART_RESOURCES  *usart)
  \brief       Control USART Interface.
  \param[in]   control  Operation
  \param[in]   arg      Argument of operation (optional)
  \param[in]   usart    Pointer to USART resources
  \return      common \ref execution_status and driver specific \ref usart_execution_status
*/
static int32_t USART_Control (      uint32_t          control,
                                    uint32_t          arg,
                              const USART_RESOURCES  *usart) {

  if ((usart->info->flags & USART_FLAG_POWERED) == 0U) {
    // USART not powered
    return ARM_DRIVER_ERROR;
  }

  switch (control & ARM_USART_CONTROL_Msk) {
    // Control break
    case ARM_USART_CONTROL_BREAK:
      return ARM_DRIVER_ERROR_UNSUPPORTED;

    // Abort Send
    case ARM_USART_ABORT_SEND:
      // TX interrupt disable
      usart->reg->CTRL &= ~CMSDK_UART_CTRL_TXIRQEN_Msk;
      // Clear tx_busy status
      usart->info->status.tx_busy = 0U;
      return ARM_DRIVER_OK;

    // Abort receive
    case ARM_USART_ABORT_RECEIVE:
      // RX interrupt disable
      usart->reg->CTRL &= ~CMSDK_UART_CTRL_RXIRQEN_Msk;
      // Clear rx_busy status
      usart->info->status.rx_busy = 0U;
      return ARM_DRIVER_OK;

    // Abort transfer
    case ARM_USART_ABORT_TRANSFER:
      // Synchronous mode not supported
      return ARM_DRIVER_ERROR_UNSUPPORTED;

    // Control TX
    case ARM_USART_CONTROL_TX:
      usart->info->flags |= USART_FLAG_TX_ENABLED;
      // Transmitter enable
      usart->reg->CTRL |= ((arg & 1U) << CMSDK_UART_CTRL_TXEN_Pos);
      return ARM_DRIVER_OK;

    // Control RX
    case ARM_USART_CONTROL_RX:
      usart->info->flags |= USART_FLAG_RX_ENABLED;
      // Receiver enable
      usart->reg->CTRL |= ((arg & 1U) << CMSDK_UART_CTRL_RXEN_Pos);
      return ARM_DRIVER_OK;
    default: break;
  }

  // Check if busy
  if ((usart->info->status.tx_busy != 0U) ||
      (usart->info->status.rx_busy != 0U) ||
     ((usart->reg->STATE & (CMSDK_UART_STATE_TXBF_Msk | CMSDK_UART_STATE_RXBF_Msk)) != 0U)) {
    return ARM_DRIVER_ERROR_BUSY;
  }

  switch (control & ARM_USART_CONTROL_Msk) {
    case ARM_USART_MODE_ASYNCHRONOUS: break;
    // Unsupported command
    default: return ARM_DRIVER_ERROR_UNSUPPORTED;
  }

  // USART Data bits
  switch (control & ARM_USART_DATA_BITS_Msk) {
    case ARM_USART_DATA_BITS_8: break;
    default: return ARM_USART_ERROR_DATA_BITS;
  }

  // USART Parity
  switch (control & ARM_USART_PARITY_Msk) {
    case ARM_USART_PARITY_NONE: break;
    default: return ARM_USART_ERROR_PARITY;
  }

  // USART Stop bits
  switch (control & ARM_USART_STOP_BITS_Msk) {
    case ARM_USART_STOP_BITS_1: break;
    default: return ARM_USART_ERROR_STOP_BITS;
  }

  // USART Flow control
  switch (control & ARM_USART_FLOW_CONTROL_Msk) {
    case ARM_USART_FLOW_CONTROL_NONE: break;
    default: return ARM_USART_ERROR_FLOW_CONTROL;
  }

  // USART Baudrate
  if ((SystemCoreClock/arg) < 16U)
    return ARM_USART_ERROR_BAUDRATE;
  else
    usart->reg->BAUDDIV = SystemCoreClock/arg;

  // Set configured flag
  usart->info->flags |= USART_FLAG_CONFIGURED;

  return ARM_DRIVER_OK;
}

/**
  \fn          ARM_USART_STATUS USART_GetStatus (const USART_RESOURCES *usart)
  \brief       Get USART status.
  \param[in]   usart     Pointer to USART resources
  \return      USART status \ref ARM_USART_STATUS
*/
static ARM_USART_STATUS USART_GetStatus (const USART_RESOURCES *usart) {
  ARM_USART_STATUS status;

  status.tx_busy          = usart->info->status.tx_busy;
  status.rx_busy          = usart->info->status.rx_busy;
  status.rx_overflow      = usart->info->status.rx_overflow;

  return status;
}

/**
  \fn          int32_t USART_SetModemControl (      ARM_USART_MODEM_CONTROL  control,
                                              const USART_RESOURCES         *usart)
  \brief       Set USART Modem Control line state.
  \param[in]   control   \ref ARM_USART_MODEM_CONTROL
  \param[in]   usart     Pointer to USART resources
  \return      \ref execution_status
*/
static int32_t USART_SetModemControl (      ARM_USART_MODEM_CONTROL  control,
                                      const USART_RESOURCES         *usart) {
  UNUSED(control);
  UNUSED(usart);

  return ARM_DRIVER_ERROR_UNSUPPORTED;
}

/**
  \fn          ARM_USART_MODEM_STATUS USART_GetModemStatus (const USART_RESOURCES *usart)
  \brief       Get USART Modem Status lines state.
  \param[in]   usart     Pointer to USART resources
  \return      modem status \ref ARM_USART_MODEM_STATUS
*/
static ARM_USART_MODEM_STATUS USART_GetModemStatus (const USART_RESOURCES *usart) {
  ARM_USART_MODEM_STATUS modem_status = { 0U, 0U, 0U, 0U };

  UNUSED(usart);

  return modem_status;
}

/**
  \fn          void USARTRX_IRQHandler (UART_RESOURCES *usart)
  \brief       USART RX Interrupt handler.
  \param[in]   usart     Pointer to USART resources
*/
static void USARTRX_IRQHandler (USART_RESOURCES *usart) {
  uint32_t status;

  status = usart->reg->INTSTATUS;
  if (usart->xfer->rx_buf != NULL)
    usart->xfer->rx_buf[usart->xfer->rx_cnt++] = (uint8_t)usart->reg->DATA;

  if (usart->xfer->rx_cnt >= usart->xfer->rx_num) {
    // RX is completed
    usart->xfer->rx_num = 0U;
    usart->info->status.rx_busy = 0U;

    // RX interrupt disable
    usart->reg->CTRL &= ~CMSDK_UART_CTRL_RXIRQEN_Msk;

    // Send event
    if (usart->info->cb_event!=NULL)
      usart->info->cb_event(ARM_USART_EVENT_RECEIVE_COMPLETE);
  }

  // clear interrupt
  usart->reg->INTCLEAR = status;
}

/**
  \fn          void USARTTX_IRQHandler (UART_RESOURCES *usart)
  \brief       USART TX Interrupt handler.
  \param[in]   usart     Pointer to USART resources
*/
static void USARTTX_IRQHandler (USART_RESOURCES *usart) {
  uint32_t status;

  // read interrupt status
  status = usart->reg->INTSTATUS;

  // clear interrupt
  usart->reg->INTCLEAR = status;

  if (usart->xfer->tx_num > 0) {

    usart->xfer->tx_cnt++;

    if (usart->xfer->tx_cnt < usart->xfer->tx_num) {
      // Send next byte
      usart->reg->DATA = usart->xfer->tx_buf[usart->xfer->tx_cnt];
    } else {
      // TX is completed
      usart->xfer->tx_num = 0U;
      usart->info->status.tx_busy = 0U;

      // TX interrupt disable
      usart->reg->CTRL &= ~CMSDK_UART_CTRL_TXIRQEN_Msk;

      // Send event
      if (usart->info->cb_event!=NULL)
        usart->info->cb_event(ARM_USART_EVENT_SEND_COMPLETE);
    }
  }

}

#if (defined (RTE_USART0) && (RTE_USART0 == 1))
// USART0 Driver Wrapper functions
       void                    UART0RX_Handler        (void);
       void                    UART0TX_Handler        (void);
static ARM_USART_CAPABILITIES  USART0_GetCapabilities (void)                                                { return USART_GetCapabilities (&USART0_Resources); }
static int32_t                 USART0_Initialize      (ARM_USART_SignalEvent_t cb_event)                    { return USART_Initialize (cb_event, &USART0_Resources); }
static int32_t                 USART0_Uninitialize    (void)                                                { return USART_Uninitialize (&USART0_Resources); }
static int32_t                 USART0_PowerControl    (ARM_POWER_STATE state)                               { return USART_PowerControl (state, &USART0_Resources); }
static int32_t                 USART0_Send            (const void *data, uint32_t num)                      { return USART_Send (data, num, &USART0_Resources); }
static int32_t                 USART0_Receive         (void *data, uint32_t num)                            { return USART_Receive (data, num, &USART0_Resources); }
static int32_t                 USART0_Transfer        (const void *data_out, void *data_in, uint32_t num)   { return USART_Transfer (data_out, data_in, num, &USART0_Resources); }
static uint32_t                USART0_GetTxCount      (void)                                                { return USART_GetTxCount (&USART0_Resources); }
static uint32_t                USART0_GetRxCount      (void)                                                { return USART_GetRxCount (&USART0_Resources); }
static int32_t                 USART0_Control         (uint32_t control, uint32_t arg)                      { return USART_Control (control, arg, &USART0_Resources); }
static ARM_USART_STATUS        USART0_GetStatus       (void)                                                { return USART_GetStatus (&USART0_Resources); }
static int32_t                 USART0_SetModemControl (ARM_USART_MODEM_CONTROL control)                     { return USART_SetModemControl (control, &USART0_Resources); }
static ARM_USART_MODEM_STATUS  USART0_GetModemStatus  (void)                                                { return USART_GetModemStatus (&USART0_Resources); }
       void                    UART0RX_Handler        (void)                                                {        USARTRX_IRQHandler (&USART0_Resources); }
       void                    UART0TX_Handler        (void)                                                {        USARTTX_IRQHandler (&USART0_Resources); }

// USART0 Driver Control Block
extern ARM_DRIVER_USART Driver_USART0;
       ARM_DRIVER_USART Driver_USART0 = {
    USARTx_GetVersion,
    USART0_GetCapabilities,
    USART0_Initialize,
    USART0_Uninitialize,
    USART0_PowerControl,
    USART0_Send,
    USART0_Receive,
    USART0_Transfer,
    USART0_GetTxCount,
    USART0_GetRxCount,
    USART0_Control,
    USART0_GetStatus,
    USART0_SetModemControl,
    USART0_GetModemStatus
};
#endif

#if (defined (RTE_USART1) && (RTE_USART1 == 1))
// USART1 Driver Wrapper functions
       void                    UART1RX_Handler        (void);
       void                    UART1TX_Handler        (void);
static ARM_USART_CAPABILITIES  USART1_GetCapabilities (void)                                                { return USART_GetCapabilities (&USART1_Resources); }
static int32_t                 USART1_Initialize      (ARM_USART_SignalEvent_t cb_event)                    { return USART_Initialize (cb_event, &USART1_Resources); }
static int32_t                 USART1_Uninitialize    (void)                                                { return USART_Uninitialize (&USART1_Resources); }
static int32_t                 USART1_PowerControl    (ARM_POWER_STATE state)                               { return USART_PowerControl (state, &USART1_Resources); }
static int32_t                 USART1_Send            (const void *data, uint32_t num)                      { return USART_Send (data, num, &USART1_Resources); }
static int32_t                 USART1_Receive         (void *data, uint32_t num)                            { return USART_Receive (data, num, &USART1_Resources); }
static int32_t                 USART1_Transfer        (const void *data_out, void *data_in, uint32_t num)   { return USART_Transfer (data_out, data_in, num, &USART1_Resources); }
static uint32_t                USART1_GetTxCount      (void)                                                { return USART_GetTxCount (&USART1_Resources); }
static uint32_t                USART1_GetRxCount      (void)                                                { return USART_GetRxCount (&USART1_Resources); }
static int32_t                 USART1_Control         (uint32_t control, uint32_t arg)                      { return USART_Control (control, arg, &USART1_Resources); }
static ARM_USART_STATUS        USART1_GetStatus       (void)                                                { return USART_GetStatus (&USART1_Resources); }
static int32_t                 USART1_SetModemControl (ARM_USART_MODEM_CONTROL control)                     { return USART_SetModemControl (control, &USART1_Resources); }
static ARM_USART_MODEM_STATUS  USART1_GetModemStatus  (void)                                                { return USART_GetModemStatus (&USART1_Resources); }
       void                    UART1RX_Handler        (void)                                                {        USARTRX_IRQHandler (&USART1_Resources); }
       void                    UART1TX_Handler        (void)                                                {        USARTTX_IRQHandler (&USART1_Resources); }

// USART1 Driver Control Block
extern ARM_DRIVER_USART Driver_USART1;
       ARM_DRIVER_USART Driver_USART1 = {
    USARTx_GetVersion,
    USART1_GetCapabilities,
    USART1_Initialize,
    USART1_Uninitialize,
    USART1_PowerControl,
    USART1_Send,
    USART1_Receive,
    USART1_Transfer,
    USART1_GetTxCount,
    USART1_GetRxCount,
    USART1_Control,
    USART1_GetStatus,
    USART1_SetModemControl,
    USART1_GetModemStatus
};
#endif

#if (defined (RTE_USART2) && (RTE_USART2 == 1))
// USART2 Driver Wrapper functions
       void                    UART2RX_Handler        (void);
       void                    UART2TX_Handler        (void);
static ARM_USART_CAPABILITIES  USART2_GetCapabilities (void)                                                { return USART_GetCapabilities (&USART2_Resources); }
static int32_t                 USART2_Initialize      (ARM_USART_SignalEvent_t cb_event)                    { return USART_Initialize (cb_event, &USART2_Resources); }
static int32_t                 USART2_Uninitialize    (void)                                                { return USART_Uninitialize (&USART2_Resources); }
static int32_t                 USART2_PowerControl    (ARM_POWER_STATE state)                               { return USART_PowerControl (state, &USART2_Resources); }
static int32_t                 USART2_Send            (const void *data, uint32_t num)                      { return USART_Send (data, num, &USART2_Resources); }
static int32_t                 USART2_Receive         (void *data, uint32_t num)                            { return USART_Receive (data, num, &USART2_Resources); }
static int32_t                 USART2_Transfer        (const void *data_out, void *data_in, uint32_t num)   { return USART_Transfer (data_out, data_in, num, &USART2_Resources); }
static uint32_t                USART2_GetTxCount      (void)                                                { return USART_GetTxCount (&USART2_Resources); }
static uint32_t                USART2_GetRxCount      (void)                                                { return USART_GetRxCount (&USART2_Resources); }
static int32_t                 USART2_Control         (uint32_t control, uint32_t arg)                      { return USART_Control (control, arg, &USART2_Resources); }
static ARM_USART_STATUS        USART2_GetStatus       (void)                                                { return USART_GetStatus (&USART2_Resources); }
static int32_t                 USART2_SetModemControl (ARM_USART_MODEM_CONTROL control)                     { return USART_SetModemControl (control, &USART2_Resources); }
static ARM_USART_MODEM_STATUS  USART2_GetModemStatus  (void)                                                { return USART_GetModemStatus (&USART2_Resources); }
       void                    UART2RX_Handler        (void)                                                {        USARTRX_IRQHandler (&USART2_Resources); }
       void                    UART2TX_Handler        (void)                                                {        USARTTX_IRQHandler (&USART2_Resources); }

// USART2 Driver Control Block
extern ARM_DRIVER_USART Driver_USART2;
       ARM_DRIVER_USART Driver_USART2 = {
    USARTx_GetVersion,
    USART2_GetCapabilities,
    USART2_Initialize,
    USART2_Uninitialize,
    USART2_PowerControl,
    USART2_Send,
    USART2_Receive,
    USART2_Transfer,
    USART2_GetTxCount,
    USART2_GetRxCount,
    USART2_Control,
    USART2_GetStatus,
    USART2_SetModemControl,
    USART2_GetModemStatus
};
#endif

#if (defined (RTE_USART3) && (RTE_USART3 == 1))
// USART3 Driver Wrapper functions
       void                    UART3RX_Handler        (void);
       void                    UART3TX_Handler        (void);
static ARM_USART_CAPABILITIES  USART3_GetCapabilities (void)                                                { return USART_GetCapabilities (&USART3_Resources); }
static int32_t                 USART3_Initialize      (ARM_USART_SignalEvent_t cb_event)                    { return USART_Initialize (cb_event, &USART3_Resources); }
static int32_t                 USART3_Uninitialize    (void)                                                { return USART_Uninitialize (&USART3_Resources); }
static int32_t                 USART3_PowerControl    (ARM_POWER_STATE state)                               { return USART_PowerControl (state, &USART3_Resources); }
static int32_t                 USART3_Send            (const void *data, uint32_t num)                      { return USART_Send (data, num, &USART3_Resources); }
static int32_t                 USART3_Receive         (void *data, uint32_t num)                            { return USART_Receive (data, num, &USART3_Resources); }
static int32_t                 USART3_Transfer        (const void *data_out, void *data_in, uint32_t num)   { return USART_Transfer (data_out, data_in, num, &USART3_Resources); }
static uint32_t                USART3_GetTxCount      (void)                                                { return USART_GetTxCount (&USART3_Resources); }
static uint32_t                USART3_GetRxCount      (void)                                                { return USART_GetRxCount (&USART3_Resources); }
static int32_t                 USART3_Control         (uint32_t control, uint32_t arg)                      { return USART_Control (control, arg, &USART3_Resources); }
static ARM_USART_STATUS        USART3_GetStatus       (void)                                                { return USART_GetStatus (&USART3_Resources); }
static int32_t                 USART3_SetModemControl (ARM_USART_MODEM_CONTROL control)                     { return USART_SetModemControl (control, &USART3_Resources); }
static ARM_USART_MODEM_STATUS  USART3_GetModemStatus  (void)                                                { return USART_GetModemStatus (&USART3_Resources); }
       void                    UART3RX_Handler        (void)                                                {        USARTRX_IRQHandler (&USART3_Resources); }
       void                    UART3TX_Handler        (void)                                                {        USARTTX_IRQHandler (&USART3_Resources); }

// USART3 Driver Control Block
extern ARM_DRIVER_USART Driver_USART3;
       ARM_DRIVER_USART Driver_USART3 = {
    USARTx_GetVersion,
    USART3_GetCapabilities,
    USART3_Initialize,
    USART3_Uninitialize,
    USART3_PowerControl,
    USART3_Send,
    USART3_Receive,
    USART3_Transfer,
    USART3_GetTxCount,
    USART3_GetRxCount,
    USART3_Control,
    USART3_GetStatus,
    USART3_SetModemControl,
    USART3_GetModemStatus
};
#endif

#endif