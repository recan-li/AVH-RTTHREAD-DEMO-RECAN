#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <errno.h>
#if 0
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#endif

#define CFG_SIGNAL_ENABLE 			0
#define CFG_PTHREAD_ENABLE 			0

#if (CFG_SIGNAL_ENABLE)
#include <signal.h>
#endif

#if (CFG_PTHREAD_ENABLE)
#include <pthread.h>
#else
//#include "iot_config.h"
//#include "platform/iot_platform_types_freertos.h"
//#include "platform/iot_threads.h"
//#include "types/iot_platform_types.h"
//#include "cmsis_os2.h"
#include "rtthread.h"
#include <rtdevice.h>
#include "msh.h"
#endif

#if (CFG_AWS_IOT_SOCKET_ENABLE)
#include "iot_socket.h"
#define AF_INET						IOT_SOCKET_AF_INET
#define SOCK_STREAM 				IOT_SOCKET_SOCK_STREAM
#define socket(af, type, protocol) 	iotSocketCreate(af, type, protocol)
#define listen(fd, backlog) 		iotSocketListen (fd, backlog)
#define read(fd, buf, len) 			iotSocketRecv(fd, buf, len)
#define write(fd, buf, len) 		iotSocketSend(fd, buf, len)
#define close(fd) 					iotSocketClose(fd)
#define INET_ADDRSTRLEN 			128
#else
#include <sys/socket.h>
#include <netinet/in.h>
#endif
 
#define MAX_CLINET_NUM 10 /** 最大客户端连接数，可根据实际情况增减 */
 
/** 使用hexdump格式打印数据的利器 */
static void hexdump(const char *title, const void *data, unsigned int len)
{
    char str[160], octet[10];
    int ofs, i, k, d;
    const unsigned char *buf = (const unsigned char *)data;
    const char dimm[] = "+------------------------------------------------------------------------------+";
 
    printf("%s (%d bytes)\n", title, len);
    printf("%s\r\n", dimm);
    printf("| Offset  : 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F   0123456789ABCDEF |\r\n");
    printf("%s\r\n", dimm);
 
    for (ofs = 0; ofs < (int)len; ofs += 16) {
        d = snprintf( str, sizeof(str), "| %08x: ", ofs );
        for (i = 0; i < 16; i++) {
            if ((i + ofs) < (int)len)
                snprintf( octet, sizeof(octet), "%02x ", buf[ofs + i] );
            else
                snprintf( octet, sizeof(octet), "   " );
 
            d += snprintf( &str[d], sizeof(str) - d, "%s", octet );
        }
        d += snprintf( &str[d], sizeof(str) - d, "  " );
        k = d;
 
        for (i = 0; i < 16; i++) {
            if ((i + ofs) < (int)len)
                str[k++] = (0x20 <= (buf[ofs + i]) &&  (buf[ofs + i]) <= 0x7E) ? buf[ofs + i] : '.';
            else
                str[k++] = ' ';
        }
 
        str[k] = '\0';
        printf("%s |\r\n", str);
    }
 
    printf("%s\r\n", dimm);
}
 
/** 获取客户端的ip和端口信息 */
static int get_clinet_ip_port(int sock, char *ip_str, int len, int *port)
{
#if (CFG_AWS_IOT_SOCKET_ENABLE)
	uint16_t cli_port = 0;
	uint8_t cli_ip[4] = {0};
	uint32_t cli_ip_len = sizeof(cli_ip);
	iotSocketGetPeerName (sock, (uint8_t *)cli_ip, (uint32_t *)&cli_ip_len, (uint16_t *)&cli_port);
	snprintf(ip_str, len, "%d.%d.%d.%d", cli_ip[0], cli_ip[1], cli_ip[2], cli_ip[3]);
	*port = cli_port;
#else
    struct sockaddr_in sa;
    int sa_len;
	
    sa_len = sizeof(sa);
    if(!getpeername(sock, (struct sockaddr *)&sa, &sa_len)) {
        *port = ntohs(sa.sin_port);
        snprintf(ip_str, len, "%s", inet_ntoa(sa.sin_addr));
    }
#endif
    return 0;
}

static int g_client_fd = -1;

static int cm_socket_putc(struct rt_serial_device *serial, char c)
{
    write(g_client_fd, &c, 1);
    return RT_EOK;
}
 
/** 服务器端处理客户端请求数据的线程入口函数 */
static void client_deal_func(void* arg)
{
    int client_fd = *(int *)arg;

    while(1) {  
        char buf[1024] = {"hello"};
        int ret = 5;
#if 1	
        memset(buf,'\0',sizeof(buf));
        ret = read(client_fd, buf, sizeof(buf)); /* 读取客户端发送的请求数据 */
        if (ret <= 0) {
        	printf("read fail(%d) %d\n", client_fd, ret);
            break; /* 接收出错，跳出循环 */
        }
#endif
 
        //hexdump("server recv:", buf, ret);

        extern void serial_out_to_socket_start(void *putc);
        extern void serial_out_to_socket_stop();

        g_client_fd = client_fd;
        serial_out_to_socket_start(cm_socket_putc);
        msh_exec(buf, ret);
        serial_out_to_socket_stop();
        
        //break;
    }

    rt_thread_delay(10000);
	
    close(client_fd);
}
 
/** 服务器主函数入口，接受命令参数输入，指定服务器监听的端口号 */
#if 0
int main(int argc, char **argv)
#else
int avh_rtt_debug_server_main(int argc, const char *argv[])
#endif
{
    int ret;
    int ser_port0 = 0;
    int ser_port1 = 0;
    int ser_port = 0;
    int ser_sock = -1;
    int client_sock = -1;
#if !(CFG_AWS_IOT_SOCKET_ENABLE)	
    struct sockaddr_in server_socket;
    struct sockaddr_in socket_in;
#endif
#if (CFG_PTHREAD_ENABLE)
    pthread_t thread_id; 
#endif 
    int val = 1;
	
#if (CFG_SIGNAL_ENABLE)
	signal(SIGPIPE,SIG_IGN);
#endif
	
    /* 命令行参数的简单判断和help提示 */
    if(argc != 3) {
        printf("usage: %s [port0 port1]\n", argv[0]);
        ret = -1;
        goto exit_entry;
    }
	
    /* 读取命令行输入的服务器监听的端口 */
    ser_port0 = atoi(argv[1]);
    ser_port1 = atoi(argv[2]);
    if (ser_port0 <=0 || ser_port0 >= 65536 || ser_port1 <=0 || ser_port1 >= 65536) {
        printf("server port error: %d %d\n", ser_port0, ser_port1);
        ret = -2;
        goto exit_entry;
    }
	
    /* 创建socket套接字 */
    ser_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(ser_sock < 0) {
        perror("socket error");
        return -3;
    }
		
#if !(CFG_AWS_IOT_SOCKET_ENABLE)
    /* 设置socket属性，使得服务器使用的端口，释放后，别的进程立即可重复使用该端口 */
    ret = setsockopt(ser_sock, SOL_SOCKET,SO_REUSEADDR, (void *)&val, sizeof(val));
    if(ret == -1) {
        perror("setsockopt");
        return -4;
    }
#endif

#if (CFG_AWS_IOT_SOCKET_ENABLE)
    uint8_t ip[4] = { 0U, 0U, 0U, 0U };
    ser_port = ser_port0;
    if((ret = iotSocketBind (ser_sock, (const uint8_t *)ip, sizeof(ip), ser_port0)) < 0)
#else
    bzero(&server_socket, sizeof(server_socket));
    server_socket.sin_family = AF_INET;
    server_socket.sin_addr.s_addr = htonl(INADDR_ANY); //表示本机的任意ip地址都处于监听
    server_socket.sin_port = htons(ser_port0);
	
    /* 绑定服务器信息 */
    if(bind(ser_sock, (struct sockaddr*)&server_socket, sizeof(struct sockaddr_in)) < 0)
#endif
    {
    	printf("bind ret: %d %d %d\n", ret, ser_sock, ser_port0);
        if((ret = iotSocketBind (ser_sock, (const uint8_t *)ip, sizeof(ip), ser_port1)) < 0) {   
            printf("bind ret: %d %d %d\n", ret, ser_sock, ser_port1);
            ret = -5;
            goto exit_entry;  
        }
        ser_port = ser_port1;
    }
	
    /* 设置服务器监听客户端的最大数目 */
    if(listen(ser_sock, MAX_CLINET_NUM) < 0) { 
        perror("listen error");
        ret = -6;
        goto exit_entry;
    }
	
    printf("Debug server create success, accepting clients @ port %d ...\n", ser_port);

    for(;;) { /* 循环等待客户端的连接 */
        char buf_ip[INET_ADDRSTRLEN];
        //printf("accepting ...\n");     
#if (CFG_AWS_IOT_SOCKET_ENABLE)
        uint32_t ip_len = sizeof(buf_ip);
        uint16_t port = 0;
        client_sock = iotSocketAccept (ser_sock, (uint8_t *)buf_ip, &ip_len, &port);
#else
        socklen_t len = 0;
        client_sock = accept(ser_sock, (struct sockaddr*)&socket_in, &len);
#endif
        if(client_sock < 0) {
            perror("accept error");
            ret = -7;
            continue;
        } else {
        	//printf("accepted fd %d ...\n", client_sock);
        }		
        
        {
            char client_ip[128];
            int client_port;
            get_clinet_ip_port(client_sock, client_ip, sizeof(client_ip), &client_port);
            /* 打印客户端的ip和端口信息 */
            //printf("client connected [ip: %s, port :%d]\n", client_ip, client_port);
        }

#if (CFG_PTHREAD_ENABLE)
        pthread_create(&thread_id, NULL, (void *)client_deal_func, (void *)&client_sock);  
        pthread_detach(thread_id);
#else
        rt_thread_t tid = rt_thread_create("debug-client", client_deal_func, (void *)&client_sock,
                                    RT_MAIN_THREAD_STACK_SIZE, RT_MAIN_THREAD_PRIORITY, 20);    
        rt_thread_startup(tid);
#endif
    }
	
exit_entry:
    if (ser_sock >= 0) {
        close(ser_sock); /* 程序退出前，释放socket资源 */
    }

    return 0;
}