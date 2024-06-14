import socket
import sys
import json
import socket
import os
import shutil
import binascii

debug_mode = False

tcp_mode = True

# 定义服务器的IP地址和端口号
SERVER_IP_ADDRESS = "127.0.0.1"
if tcp_mode:
    TCP_PORT_NO0 = 18888
    TCP_PORT_NO1 = 18889
else:
    UDP_PORT_NO = 55556

'''
// register auth ok qrcode
static char *g_auth_ok_qrcoe_list[] = 
{
    "130608135571250081",
};
'''
g_auth_ok_qrcoe_list = ["130608135571250081"]

# 创建一个UDP socket
if tcp_mode:
    serverSock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
else:
    serverSock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

if tcp_mode:
    try: 
        # Server address and port
        server_address = (SERVER_IP_ADDRESS, TCP_PORT_NO0)
        # Bind the socket to the port
        if debug_mode:
            print(f"Starting up on {server_address[0]} port {server_address[1]}")
        serverSock.bind(server_address)
    except Exception as e:
        # Server address and port
        server_address = (SERVER_IP_ADDRESS, TCP_PORT_NO1)
        # Bind the socket to the port
        if debug_mode:
            print(f"Starting up on {server_address[0]} port {server_address[1]}")
        serverSock.bind(server_address)
else:
    # 绑定socket到指定的IP地址和端口号
    serverSock.bind((SERVER_IP_ADDRESS, UDP_PORT_NO))

file_fd = None

if tcp_mode:
    # Listen for incoming connections
    serverSock.listen(1)

while True:
    if tcp_mode:
        if debug_mode:
            print("Waiting for a new connection...")
        connection, client_address = serverSock.accept()
        #print(connection)
        #print(client_address)

    #print('-------------1111')
    if tcp_mode:
        data = connection.recv(10240)
    else:
        data, addr = serverSock.recvfrom(10240)

    '''
    {
        "qrcode": "1234567",
        "result": "ok",
    }
    '''

    try:
        qrcode = ""
        data_str = data.decode('utf-8').strip()
        if debug_mode:
            print(data_str)
        parsed_data = json.loads(data_str)
        qrcode = parsed_data.get('qrcode')
        #print(qrcode)
    except Exception as e:
        rsp_content = str(e)

    try:
        if qrcode in g_auth_ok_qrcoe_list:
            rsp_content = "{\"qrcode\":\"" + qrcode + "\", \"result\": \"ok\"}"
        
        else:
            rsp_content = "{\"qrcode\":\"" + qrcode + "\", \"result\": \"fail\"}"
        
    except Exception as e:
        rsp_content = str(e)


    if tcp_mode:
        connection.sendall(rsp_content.encode('utf-8'))
        connection.close()
    else:
        serverSock.sendto(rsp_content.encode('utf-8'), addr)
        serverSock.close()
    
    if debug_mode:
        print(rsp_content)
        print("")
