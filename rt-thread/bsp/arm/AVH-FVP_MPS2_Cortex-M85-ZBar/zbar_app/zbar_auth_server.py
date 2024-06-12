import socket
import sys
import json
import socket
import os
import shutil
import binascii

debug_mode = True

tcp_mode = True

# 定义服务器的IP地址和端口号
SERVER_IP_ADDRESS = "127.0.0.1"
if tcp_mode:
    TCP_PORT_NO = 18888
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

# 假设这是你要解析的JSON字符串
json_string = '{"qrcode": "130608135571250082"}'  # 这里应该是你要解析的JSON字符串

# 尝试解析JSON字符串
try:
    parsed_data = json.loads(json_string)
    qrcode_number = parsed_data.get('qrcode')
    print(qrcode_number)
except json.JSONDecodeError as e:
    print(f"Error parsing JSON: {e}")
except Exception as e:
    print(f"An error occurred: {e}")

# 创建一个UDP socket
if tcp_mode:
    serverSock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
else:
    serverSock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

if tcp_mode:
    # Server address and port
    server_address = (SERVER_IP_ADDRESS, TCP_PORT_NO)
    # Bind the socket to the port
    print(f"Starting up on {server_address[0]} port {server_address[1]}")
    serverSock.bind(server_address)
else:
    # 绑定socket到指定的IP地址和端口号
    serverSock.bind((SERVER_IP_ADDRESS, UDP_PORT_NO))

file_fd = None

if tcp_mode:
    # Listen for incoming connections
    serverSock.listen(1)
    print("Waiting for a connection...")
    connection, client_address = serverSock.accept()
    #print(connection)
    #print(client_address)

    # JSON字符串
    json_string = '{"qrcode": "130608135571250082"}'

    # 解析JSON字符串
    data = json.loads(json_string)

    # 提取数字
    qrcode_number = data.get('qrcode')

    # 打印结果
    print(qrcode_number)  # 输出: 130608135571250082

while True:
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

    qrcode = ""
    print(data.decode('utf-8').strip())
    # 使用json.loads()函数解析JSON字符串
    #try:
    json_string = data.decode('utf-8').strip()
    #print(json_string)
    parsed_data = json.loads(json_string)
    print(parsed_data)
    qrcode = parsed_data.get('qrcode')
    #except Exception as e:
    #print(e)
    #print("", end=" ")

    try:
        if qrcode in g_auth_ok_qrcoe_list:
            rsp_content = "{\"result\": \"ok\"}"
        
        else:
            rsp_content = "{\"result\": \"fail\"}"
        
    except Exception as e:
        rsp_content = str(e)

    if tcp_mode:
        connection.sendall(rsp_content.encode('utf-8'))
    else:
        serverSock.sendto(rsp_content.encode('utf-8'), addr)
    if debug_mode:
        print("")
