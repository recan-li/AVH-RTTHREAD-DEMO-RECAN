/*
 * Copyright (c) 2006-2023, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes
 * 2021-07-29     KyleChan          first version
 * 2023-10-17     Rbb666            add ra8 adapt
 * 2024-03-11    Wangyuqiang        add rzt2m adapt
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <rthw.h>

#include <stdio.h>

#include "drv_usart.h"

#ifdef RT_USING_SERIAL_V2

//#define DRV_DEBUG
#define DBG_TAG                 "drv.usart"
#ifdef DRV_DEBUG
    #define DBG_LVL             DBG_LOG
#else
    #define DBG_LVL             DBG_INFO
#endif /* DRV_DEBUG */
#include <rtdbg.h>

static struct rt_serial_device serial0;

extern int stdout_putchar (int ch);
extern int stdin_getchar (void);

/*
 * UART interface
 */
static rt_err_t cm_uart_configure(struct rt_serial_device *serial, struct serial_configure *cfg)
{
    //TODO
    return RT_EOK;
}

static rt_err_t cm_uart_control(struct rt_serial_device *serial, int cmd, void *arg)
{
    //TODO
    return RT_EOK;
}

static int cm_uart_putc(struct rt_serial_device *serial, char c)
{
    //printf("%c", c);
    stdout_putchar(c);
    return RT_EOK;
}

static int cm_uart_getc(struct rt_serial_device *serial)
{
    int c = stdin_getchar();
    return c;
}

static rt_ssize_t cm_uart_transmit(struct rt_serial_device     *serial,
                                   rt_uint8_t           *buf,
                                   rt_size_t             size,
                                   rt_uint32_t           tx_flag)
{
    //TODO
    return size;
}

static struct rt_uart_ops cm_uart_ops =
{
    .configure  = cm_uart_configure,
    .control    = cm_uart_control,
    .putc       = cm_uart_putc,
    .getc       = cm_uart_getc,
    .transmit   = cm_uart_transmit
};

typedef int (*seiral_putc)(struct rt_serial_device *serial, char c);

void serial_out_to_socket_start(void *putc)
{
    rt_interrupt_enter();
    cm_uart_ops.putc = (seiral_putc)putc;
    rt_interrupt_leave();
}

void serial_out_to_socket_stop(void)
{
    rt_interrupt_enter();
    cm_uart_ops.putc = cm_uart_putc;
    rt_interrupt_leave();
}

// USART中断服务例程
void UART0RX_Handler_User(void) 
{
    rt_interrupt_enter();

    printf("xxx");

    struct rt_serial_device *serial = &serial0;
    RT_ASSERT(serial != RT_NULL);

    struct rt_serial_rx_fifo *rx_fifo;
    rx_fifo = (struct rt_serial_rx_fifo *) serial->serial_rx;
    RT_ASSERT(rx_fifo != RT_NULL);

    // 检查是否是接收中断
    if (ptrUSART->GetStatus().rx_busy != 0) {
        // 准备一个缓冲区来接收数据
        uint8_t data[1]; // 缓冲区大小为1字节
        // 读取接收到的数据
        int32_t status = ptrUSART->Receive(data, 1);
        
        if (status == ARM_DRIVER_OK) {
            // 处理接收到的数据...  
            printf("rx: %d\n", data[0]);
            rt_ringbuffer_putchar(&(rx_fifo->rb), (rt_uint8_t)data[0]);
            rt_hw_serial_isr(serial, RT_SERIAL_EVENT_RX_IND);            
        } else {
            // 处理错误...
        }
    }

    rt_interrupt_leave();
}

extern int stdio_init (void);
extern int stdout_puts(const char *s);

int rt_hw_usart_init(void)
{
    int32_t status;
    rt_err_t result = 0;

    stdio_init();

    /* init UART object */
    serial0.config.rx_bufsz = BSP_UART0_RX_BUFSIZE;
    serial0.config.tx_bufsz = BSP_UART0_TX_BUFSIZE;

    serial0.ops = &cm_uart_ops;

    /* register UART device */
    result = rt_hw_serial_register(&serial0,
                                   "uart0",
                                   RT_DEVICE_FLAG_RDWR,
                                   NULL);
    RT_ASSERT(result == RT_EOK);

    return result;
}

#endif /* RT_USING_SERIAL_V2 */

void rt_hw_console_output(const char *str)
{
    stdout_puts(str);
}
