/*
 * Copyright (c) 2006-2023, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-10-10      Sherman      first version
 */

#ifndef __BOARD_H__
#define __BOARD_H__

#ifdef __cplusplus
extern "C" {
#endif

/* refer to VHT_Corstone_SSE-310.sct */
#define AVH_CM85_SRAM_SIZE    	768 /* The SRAM size of the chip needs to be modified */
#define AVH_CM85_SRAM_END     	(0x11100000 + AVH_CM85_SRAM_SIZE * 1024)

#ifdef __ARMCC_VERSION
extern int Image$$ARM_LIB_HEAP$$ZI$$Base;
extern int Image$$ARM_LIB_HEAP$$ZI$$Limit;
#define HEAP_BEGIN  	((void *)&Image$$ARM_LIB_HEAP$$ZI$$Base)
#define HEAP_END        ((void *)&Image$$ARM_LIB_HEAP$$ZI$$Limit)
#elif __ICCARM__
#pragma section="CSTACK"
#define HEAP_BEGIN      (__segment_end("CSTACK"))
#define HEAP_END        // TODO
#else
extern int __RAM_segment_used_end__;
#define HEAP_BEGIN      (&__RAM_segment_used_end__)
#define HEAP_END        // TODO
#endif

#ifdef __cplusplus
}
#endif

#endif
