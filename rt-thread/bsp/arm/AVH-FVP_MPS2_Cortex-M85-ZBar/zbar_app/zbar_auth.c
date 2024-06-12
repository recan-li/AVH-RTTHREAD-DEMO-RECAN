
#include <stdio.h>
#include <string.h>

#include "rtthread.h"
#include "rtdef.h"
#include "iot_socket.h"
#include "cJSON.h"

#define ARRAY_SIZE(n) (sizeof(n) / sizeof(n[0]))

#define SERVER_IP       "127.0.0.1"
#define SERVER_IP_BYTES {0x7F, 0x00, 0x00, 0x01} // sepcial for AVH socket API

#define SERVER_PORT     18888

extern const uint8_t gImage_test8[];
extern const uint8_t gImage_test9[];

// register auth ok qrcode
static char *g_auth_ok_qrcoe_list[] = 
{
    "130608135571250081",
};

int zbar_check_auth_offline(char *qrcode)
{
    int i = 0;

    for (i = 0; i < ARRAY_SIZE(g_auth_ok_qrcoe_list); i++) {
        if (!strcmp(g_auth_ok_qrcoe_list[i], qrcode)) {
            rt_kprintf("qrcode: %s auth offline ok\n", qrcode);
            return 0;
        }
    }

    rt_kprintf("qrcode: %s auth offline fail\n", qrcode);
    return 0;
}

int zbar_check_auth_online(char *qrcode)
{
	int sock;
	int ret = -1;	
    char buffer[1024];

    if ((sock = iotSocketCreate(IOT_SOCKET_AF_INET, IOT_SOCKET_SOCK_STREAM, IOT_SOCKET_IPPROTO_TCP)) < 0) {
    	rt_kprintf("socket fail\n");
    	goto exit_entry;
    }

    uint8_t ip_array[] = SERVER_IP_BYTES;
    uint32_t ip_len = sizeof(ip_array);
    ret = iotSocketConnect (sock, (const uint8_t *)ip_array, ip_len, SERVER_PORT);
    if (ret < 0) {
    	rt_kprintf("connect fail\n");
    	goto exit_entry;
    }

    //snprintf(buffer, sizeof(buffer), "{\"qrcode\" : \"%s\"}", qrcode);
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "qrcode", cJSON_CreateString(qrcode));
    char *json_string = cJSON_PrintUnformatted(root);

    ret = iotSocketSend(sock, json_string, strlen(json_string));
    if (ret < 0) {
    	rt_kprintf("send fail\n");
    	goto exit_entry;
    }

    free(json_string);
    cJSON_Delete(root);

    ret = iotSocketRecv (sock, buffer, sizeof(buffer));
    if (ret < 0) {
    	rt_kprintf("recv fail\n");
    	goto exit_entry;
    }

    cJSON * rsp_root = NULL;
    cJSON * result = NULL;

    rsp_root = cJSON_Parse(buffer);
    result = cJSON_GetObjectItem(rsp_root, "result");

    if (!strcmp(result->valuestring, "ok")) {
        rt_kprintf("qrcode: %s auth online ok\n", qrcode);
        ret = 0;
    } else {
    	rt_kprintf("qrcode: %s auth online fail\n", qrcode);
    }

    cJSON_Delete(rsp_root);

exit_entry:
	if (sock >= 0) {
		iotSocketClose(sock);
	}
	
    return ret;
}

extern int get_qrcode_text(void *raw, int width, int height, char *qrcode, int debug);

static char auth_cmd[128];

void zbar_qrcode_get_thread_entry(void *arg)
{
    char *cmd = (char *)arg;
    char qrcode[64] = {0};
        
    //rt_kprintf("zbar qrcode auth start ... %s\n", cmd);
    if (!strcmp(cmd, "offline_ok_test")) {
        // auth for pic: './qrcode_bmp/wx-qrcode-130608135571250081-24-128x128.bmp'
        get_qrcode_text((void *)gImage_test8, 128, 128, qrcode, 0);
        zbar_check_auth_offline(qrcode);   
    } else if (!strcmp(cmd, "offline_fail_test")) {
        // auth for pic: './qrcode_bmp/wx-qrcode-130608135571250082-24-128x128.bmp'
        get_qrcode_text((void *)gImage_test9, 128, 128, qrcode, 0);
        zbar_check_auth_offline(qrcode); 
    } else if (!strcmp(cmd, "online_ok_test")) {
        // auth for pic: './qrcode_bmp/wx-qrcode-130608135571250081-24-128x128.bmp'
        get_qrcode_text((void *)gImage_test8, 128, 128, qrcode, 0);
        zbar_check_auth_online(qrcode);   
    } else if (!strcmp(cmd, "online_fail_test")) {
        // auth for pic: './qrcode_bmp/wx-qrcode-130608135571250082-24-128x128.bmp'
        get_qrcode_text((void *)gImage_test9, 128, 128, qrcode, 0);
        zbar_check_auth_online(qrcode); 
    } else {
        rt_kprintf("invailed parameter\n");
    }
}

void zbar_qrcode_get_thread_start(char *cmd)
{
    rt_thread_t tid;

    tid = rt_thread_create("zbar_qrcode", zbar_qrcode_get_thread_entry, cmd, 4096, RT_MAIN_THREAD_PRIORITY / 2, 10);
    rt_thread_startup(tid);
}

int zbar_qrcode_auth(int argc, char** argv)
{
    if(argc == 2)
    {
        snprintf(auth_cmd, sizeof(auth_cmd), "%s", argv[1]);
        zbar_qrcode_get_thread_start(auth_cmd);    
    } else {
        rt_kprintf("invailed parameter\n");
    }

    return 0;
}
MSH_CMD_EXPORT(zbar_qrcode_auth, auth qrcode which decoded by zbar);
