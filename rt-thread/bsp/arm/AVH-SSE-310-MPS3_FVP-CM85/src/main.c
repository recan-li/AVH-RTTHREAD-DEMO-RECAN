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

#if (CFG_RTT_ENABLE)
#include <rtthread.h>
#endif

int main(void) 
{
#if !(CFG_RTT_ENABLE)
	stdout_init();
#endif

	while (1) {
		extern unsigned long int g_cnt;
		if (g_cnt % 1000 == 0) {
			printf("===%d\n", g_cnt);
		}
	}

	int cnt = 1;

	while (1)
	{
		printf("%d ...\n", cnt++);
		rt_thread_delay(1);
	}

	printf("Hello AVH-CM85 ...\n");

	printf("EXITTHESIM\r\n");
  	return 0;
}

void main_thread_entry2(void *parameter)
{
	int cnt = 1;

	while (1)
	{
		printf("===== %d ...\n", cnt++);
		rt_thread_delay(10);
	}
}
