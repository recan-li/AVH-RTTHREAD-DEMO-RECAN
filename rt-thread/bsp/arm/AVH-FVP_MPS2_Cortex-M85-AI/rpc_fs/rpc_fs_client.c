#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "cJSON.h"
#include "rpc_fs.h"

#if (CFG_AWS_IOT_SOCKET_ENABLE)
#include "iot_socket.h"
#define AF_INET                     IOT_SOCKET_AF_INET
#define SOCK_STREAM                 IOT_SOCKET_SOCK_STREAM
#define SOCK_DGRAM                  IOT_SOCKET_SOCK_DGRAM
#define socket(af, type, protocol)  iotSocketCreate(af, type, protocol)
#define listen(fd, backlog)         iotSocketListen (fd, backlog)
#define read(fd, buf, len)          iotSocketRecv(fd, buf, len)
#define write(fd, buf, len)         iotSocketSend(fd, buf, len)
#define close(fd)                   iotSocketClose(fd)
#define INET_ADDRSTRLEN             128
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#define TCP_MODE        0

#define SERVER_IP       "127.0.0.1"
#define SERVER_IP_BYTES {0x7F, 0x00, 0x00, 0x01} // sepcial for AVH socket API

#if (TCP_MODE)
#define SERVER_PORT     55555
#else
#define SERVER_PORT     55556
#endif

static int g_sock = -1;
static int g_fd = 1;
#if !(CFG_AWS_IOT_SOCKET_ENABLE)
static struct sockaddr_in g_serv_addr;
#endif

int rpc_fs_init(void)
{
#if (CFG_AWS_IOT_SOCKET_ENABLE)
#if (TCP_MODE)
    if ((g_sock = iotSocketCreate(AF_INET, IOT_SOCKET_SOCK_STREAM, IOT_SOCKET_IPPROTO_TCP)) < 0) {
#else
    if ((g_sock = iotSocketCreate(AF_INET, IOT_SOCKET_SOCK_DGRAM, IOT_SOCKET_IPPROTO_UDP)) < 0) {
#endif
        return -1;
    }
    //printf("g_sock: %d\n", g_sock);    
#else
    if ((g_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        return -1;
    }

    memset(&g_serv_addr, 0, sizeof(g_serv_addr));
    g_serv_addr.sin_family = AF_INET;
    g_serv_addr.sin_port = htons(SERVER_PORT);
    g_serv_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
#endif

#if (TCP_MODE)
#if (CFG_AWS_IOT_SOCKET_ENABLE)
    uint8_t ip_array[] = SERVER_IP_BYTES;
    uint32_t ip_len = sizeof(ip_array);
    int ret = iotSocketConnect (g_sock, (const uint8_t *)ip_array, ip_len, SERVER_PORT);
    //printf("ret: %d\n", ret);
#else
    if (connect(g_sock, (struct sockaddr *)&g_serv_addr, sizeof(g_serv_addr)) < 0) {
        close(g_sock);
        printf("connect fail\n");
        return -1;
    }
#endif
#endif
    printf("g_sock1: %d\n", g_sock);
    return g_sock;
}

int rpc_fs_deinit(void)
{
    close(g_sock);
    return 0;
}

cJSON *rpc_fs_cJSON_exhcange(cJSON *in, int exp_len)
{
    char *json_string = cJSON_Print(in);
    if (json_string) {
        if (strlen(json_string) <= 2048) {
            printf(">>> %s\n", json_string);
        }
#if (CFG_AWS_IOT_SOCKET_ENABLE)
        //printf("%s:%d %d\n", __func__, __LINE__, g_sock);
        //extern int32_t iotSocketSendTo (int32_t socket, const void *buf, \
            uint32_t len, const uint8_t *ip, uint32_t ip_len, uint16_t port);
#if (TCP_MODE)
        int ret = iotSocketSend(g_sock, json_string, strlen(json_string));
#else
        uint8_t ip_array[] = SERVER_IP_BYTES;
        uint32_t ip_len = sizeof(ip_array);
        //printf("%d %d %d %d %d\n", ip_array[0], ip_array[1], ip_array[2], ip_array[3], ip_len);
        int ret = iotSocketSendTo (g_sock, json_string, strlen(json_string), \
            (const uint8_t *)ip_array, ip_len, SERVER_PORT);
#endif
        //printf("%s:%d %d\n", __func__, __LINE__, ret);

#else
#if (TCP_MODE)
        send(g_sock, json_string, strlen(json_string), 0);
#else
        sendto(g_sock, json_string, strlen(json_string), 0, \
            (struct sockaddr *)&g_serv_addr, sizeof(g_serv_addr));
#endif
#endif
        free(json_string);
    }
    
    // 释放 cJSON 对象
    cJSON_Delete(in);

    char buffer[1024] = {0};
    int recv_size = sizeof(buffer);
    char *p_buffer = buffer;

    if (exp_len > recv_size) {
        recv_size = exp_len * 2 + 100;
        p_buffer = (char *)malloc(recv_size);        
    }     

#if (CFG_AWS_IOT_SOCKET_ENABLE)
    uint8_t ip_recv[64];
    uint16_t port = 0;
    uint32_t ip_len = sizeof(ip_recv);
#if (TCP_MODE)
    int ret = iotSocketRecv (g_sock, p_buffer, recv_size);
#else
    int ret = iotSocketRecvFrom (g_sock, p_buffer, recv_size, ip_recv, &ip_len, &port);
#endif
#else
#if (TCP_MODE)
    int ret = recv(g_sock, p_buffer, recv_size, 0);
#else
    int ret = recvfrom(g_sock, p_buffer, recv_size, 0, NULL, NULL);
#endif
#endif
    printf("ret: %d %d\n", ret, recv_size);
    if (p_buffer == buffer) {
        printf("<<< %s\n", p_buffer);
    }

    cJSON * rsp_ret = NULL;

    if (ret < 0) {
        goto exit_entry;
    }

    cJSON * rsp_root = NULL;
    cJSON * status = NULL;
    RPC_FILE *file = NULL;

    rsp_root = cJSON_Parse(p_buffer);
    status = cJSON_GetObjectItem(rsp_root, "status");

    if (strcmp(status->valuestring, "ok")) {
        printf("status err: %s\n", status->valuestring);
        cJSON_Delete(rsp_root);
        goto exit_entry;
    } 

    rsp_ret = rsp_root;

exit_entry:

    if (p_buffer != buffer) {
        free(p_buffer);
        p_buffer = NULL;
    }

    return rsp_ret;
}

/* FILE *fopen(const char *pathname, const char *mode); */
RPC_FILE * rpc_fs_fopen(const char *pathname, const char *mode)
{
    RPC_FILE *file = NULL;
    cJSON *root = cJSON_CreateObject();
    
    // 添加键值对到 cJSON 对象中
    cJSON_AddItemToObject(root, "name", cJSON_CreateString(pathname));
    cJSON_AddItemToObject(root, "operation", cJSON_CreateString("fopen"));
    cJSON_AddItemToObject(root, "mode", cJSON_CreateString(mode));

    cJSON *rsp_root = rpc_fs_cJSON_exhcange(root, 0);
    if (rsp_root) {
        file = (RPC_FILE *)&g_fd;
        cJSON_Delete(rsp_root);
    }    

    return file;
}

/*
int fseek(FILE *stream, long offset, int whence);
*/
int rpc_fs_fseek(RPC_FILE *stream, long offset, int whence)
{
    cJSON *root = cJSON_CreateObject();
    
    cJSON_AddItemToObject(root, "operation", cJSON_CreateString("fseek"));
    cJSON_AddItemToObject(root, "offset", cJSON_CreateNumber(offset));
    cJSON_AddItemToObject(root, "whence", cJSON_CreateNumber(whence));

    cJSON *rsp_root = rpc_fs_cJSON_exhcange(root, 0);
    if (rsp_root) {
        cJSON_Delete(rsp_root);
        return 0;
    } else {
        return -1;
    }
}

/*
long ftell(FILE *stream);
*/
long rpc_fs_ftell(RPC_FILE *stream)
{
    cJSON *root = cJSON_CreateObject();
    
    cJSON_AddItemToObject(root, "operation", cJSON_CreateString("ftell"));

    cJSON *rsp_root = rpc_fs_cJSON_exhcange(root, 0);
    if (rsp_root) {
        cJSON *offset = cJSON_GetObjectItem(rsp_root, "offset");
        cJSON_Delete(rsp_root);
        return offset->valueint;
    } else {
        return -1;
    }
}

// 将单个十六进制字符转换为对应的整数值
unsigned char hex_char_to_int(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    } else if (c >= 'a' && c <= 'f') {
        return c - 'a' + 10;
    } else if (c >= 'A' && c <= 'F') {
        return c - 'A' + 10;
    }
    return 0; // 处理非法字符，默认0
}

// 将十六进制字符串转换为字节数组
void hex_string_to_byte_array(const char* hex_string, unsigned char* byte_array) 
{
    size_t len = strlen(hex_string);
    size_t i;
    for (i = 0; i < len; i += 2) {
        byte_array[i/2] = (hex_char_to_int(hex_string[i]) << 4) + \
            hex_char_to_int(hex_string[i+1]);
    }
}

//将二进制源串分解成双倍长度可读的16进制串, 如 0x12AB-->"12AB"
void byte_array_to_hex_string(uint8_t *psIHex, int32_t iHexLen, char *psOAsc)
{
    static const char szMapTable[17] = {"0123456789ABCDEF"};
    int32_t   iCnt,index;
    unsigned char  ChTemp;
 
    for(iCnt = 0; iCnt < iHexLen; iCnt++) {
        ChTemp = (unsigned char)psIHex[iCnt];
        index = (ChTemp / 16) & 0x0F;
        psOAsc[2*iCnt]   = szMapTable[index];
        ChTemp = (unsigned char) psIHex[iCnt];
        index = ChTemp & 0x0F;
        psOAsc[2*iCnt + 1] = szMapTable[index];
    }
}

/*
size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
*/
size_t rpc_fs_fread(void *ptr, size_t size, size_t nmemb, RPC_FILE *stream)
{
    int read_size = size * nmemb;
    cJSON *root = cJSON_CreateObject();
    
    cJSON_AddItemToObject(root, "operation", cJSON_CreateString("fread"));
    cJSON_AddItemToObject(root, "length", cJSON_CreateNumber(read_size));

    cJSON *rsp_root = rpc_fs_cJSON_exhcange(root, read_size);
    if (rsp_root) {
        cJSON *contents = cJSON_GetObjectItem(rsp_root, "contents");
        //printf("contents: %s\n", contents->valuestring);
        hex_string_to_byte_array(contents->valuestring, ptr);
        cJSON_Delete(rsp_root);
        return read_size;
    } else {
        return -1;
    }  
}

/*
size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);
*/
size_t rpc_fs_fwrite(const void *ptr, size_t size, size_t nmemb, RPC_FILE *stream)
{
    int write_size = size * nmemb;
    cJSON *root = cJSON_CreateObject();
    uint8_t *contents_hex_string = malloc(write_size * 2);

    if (!contents_hex_string) {
        return -1;
    }
    
    byte_array_to_hex_string((uint8_t *)ptr, write_size, (char *)contents_hex_string);
    cJSON_AddItemToObject(root, "operation", cJSON_CreateString("fwrite"));
    cJSON_AddItemToObject(root, "contents", cJSON_CreateString((const char *)contents_hex_string));
    cJSON_AddItemToObject(root, "length", cJSON_CreateNumber(write_size));

    cJSON *rsp_root = rpc_fs_cJSON_exhcange(root, 0);
    if (rsp_root) {
        cJSON_Delete(rsp_root);
        return write_size;
    } else {
        return -1;
    }
}

/*
int fclose(FILE *stream);
*/
int rpc_fs_fclose(RPC_FILE *file)
{
    cJSON *root = cJSON_CreateObject();
    
    cJSON_AddItemToObject(root, "operation", cJSON_CreateString("fclose"));

    cJSON *rsp_root = rpc_fs_cJSON_exhcange(root, 0);
    if (rsp_root) {
        cJSON_Delete(rsp_root);
        return 0;
    } else {
        return -1;
    }
}

int rpc_fs_fsize(const char *file_name)
{
    RPC_FILE *file = rpc_fs_fopen(file_name, "rb+");

    rpc_fs_fseek(file, 0, SEEK_END);

    int offset = rpc_fs_ftell(file);
    
    rpc_fs_fclose(file);

    return offset;
}

#ifndef MAIN
int rpc_fs_main(int argc, const char *argv[])
#else
int main(int argc, const char *argv[])
#endif
{    
    int offset = 0;
    char buffer[1024] = {0};
    const char *file_name = "123.txt";
    char *write_request = "WRITE test.txt";
    RPC_FILE *file = NULL;

    if (argc > 1) {
        file_name = argv[1];
    }

    rpc_fs_init();

    printf("%d %d %d\n", SEEK_SET, SEEK_CUR, SEEK_END);

    int size = rpc_fs_fsize("123.txt");
    printf("size: %d\n", size);
    size = rpc_fs_fsize("456.txt");
    printf("size: %d\n", size);

    file = rpc_fs_fopen(file_name, "rb+");

    offset = rpc_fs_ftell(file);
    printf("offset1: %d\n", offset);

    rpc_fs_fseek(file, 10, SEEK_CUR);

    offset = rpc_fs_ftell(file);
    printf("offset2: %d\n", offset);   

    rpc_fs_fread(buffer, 1, 10, file);
    printf("buf: %s\n", buffer);

    rpc_fs_fseek(file, 5, SEEK_SET);

    offset = rpc_fs_ftell(file);
    printf("offset3: %d\n", offset);   

    rpc_fs_fread(buffer, 1, 10, file);
    printf("buf: %s\n", buffer);

    rpc_fs_fseek(file, 5, SEEK_SET);

    offset = rpc_fs_ftell(file);
    printf("offset4: %d\n", offset);   

    rpc_fs_fwrite(write_request, 1, 10, file);

    rpc_fs_fseek(file, 5, SEEK_SET);

    offset = rpc_fs_ftell(file);
    printf("offset5: %d\n", offset);   

    rpc_fs_fread(buffer, 1, 10, file);
    printf("buf: %s\n", buffer);

    rpc_fs_fclose(file);

    rpc_fs_deinit();

    return 0;
}
