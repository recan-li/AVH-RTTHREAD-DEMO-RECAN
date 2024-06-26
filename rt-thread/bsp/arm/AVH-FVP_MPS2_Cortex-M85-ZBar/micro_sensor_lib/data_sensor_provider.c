/* Copyright 2018 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include <stdio.h>
#include <string.h>

#include "app_cfg.h"
#include "data_sensor_provider.h"
#include "sensor_drv.h"
//#include "cmsis_os2.h"

#define SENSOR_BLOCK_NUM (1)
#define SENSOR_BLOCK_SIZE (DATA_NUM_ELEMENTS)
#define SENSOR_BUFFER_SIZE (SENSOR_BLOCK_NUM * SENSOR_BLOCK_SIZE)

#ifdef __FVP_PY
    __attribute__((section(".ARM.__at_0x9FFF0000")))
#endif
    __attribute__((aligned(4)))

DATA_TYPE sensor_buffer[SENSOR_BUFFER_SIZE];

uint8_t is_sensor_initialized = 0;
uint8_t is_sensor_ready = 0;
uint32_t previous_data_count = 0;
uint32_t current_data_count = 0;

void sensor_rx_event(void);

static void sensor_event(uint32_t event)
{
    if (!is_sensor_ready)
    {
        is_sensor_ready = 1;
        return;
    }
    if (event & SENSOR_DRV_EVENT_RX_DATA)
    {
        current_data_count += (SENSOR_BLOCK_SIZE);
#ifdef __EVENT_DRIVEN
        sensor_rx_event();
#endif
    }
}

static int32_t sensor_driver_setup(void)
{
    int32_t ret;

    ret = SensorDrv_Initialize(sensor_event);
    if (ret != 0)
    {
        return ret;
    }

    ret = SensorDrv_Configure(SENSOR_DRV_INTERFACE_RX,
                              1U, /* single channel */
                              sizeof(DATA_TYPE) * 8, /* sample bits */
                              DATA_SAMPLE_RATE); // sample rate 
    if (ret != 0)
    {
        return ret;
    }

    ret = SensorDrv_SetBuf(SENSOR_DRV_INTERFACE_RX,
                           sensor_buffer, SENSOR_BLOCK_NUM, SENSOR_BLOCK_SIZE);
    if (ret != 0)
    {
        return ret;
    }

    ret = SensorDrv_Control(SENSOR_DRV_CONTROL_RX_ENABLE);
    if (ret != 0)
    {
        return ret;
    }

#ifdef __GATED_FETCH
    ret = SensorDrv_Control(SENSOR_DRV_CONTROL_RX_PAUSE);
    if (ret != 0)
    {
        return ret;
    }
#endif

    return 0;
}


// TODO: we might need a timeout, if no data is retrieved before timeout, return an error code
uint32_t get_sensor_data(uint32_t num_data, uint32_t *sensor_data_size, DATA_TYPE **sensor_data)
{
    if (!is_sensor_initialized)
    {
        int32_t ret = sensor_driver_setup();
        if (ret != 0)
        {
            return 1;
        }
        is_sensor_initialized = 1;
        return 0;
    }

#ifdef __GATED_FETCH
    SensorDrv_Control(SENSOR_DRV_CONTROL_RX_RESUME);
#endif

    while(current_data_count <= previous_data_count)
    {
        rt_thread_delay(10U);
    }
#ifdef __GATED_FETCH
    SensorDrv_Control(SENSOR_DRV_CONTROL_RX_PAUSE);
#endif

    *sensor_data_size = sizeof(**sensor_data) * num_data;
    memcpy(*sensor_data, sensor_buffer, *sensor_data_size);

    previous_data_count = current_data_count;

    return 0;
}

uint32_t get_total_fetched_sensor_data()
{
    return current_data_count;
}
