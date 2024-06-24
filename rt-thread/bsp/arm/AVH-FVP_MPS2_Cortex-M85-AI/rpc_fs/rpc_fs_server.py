import socket
import sys
import json
import socket
import os
import shutil
import binascii

debug_mode = False

tcp_mode = False

# 定义服务器的IP地址和端口号
SERVER_IP_ADDRESS = "127.0.0.1"
if tcp_mode:
    TCP_PORT_NO = 55555
else:
    UDP_PORT_NO = 55556

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
    print(f"Starting up on " + SERVER_IP_ADDRESS + " port " + str(UDP_PORT_NO))
    serverSock.bind((SERVER_IP_ADDRESS, UDP_PORT_NO))

file_fd = None

if tcp_mode:
    # Listen for incoming connections
    serverSock.listen(1)
    print("Waiting for a connection...")
    connection, client_address = serverSock.accept()
    print(connection)
    print(client_address)

while True:
    if tcp_mode:
        data = connection.recv(10240)
    else:
        data, addr = serverSock.recvfrom(10240)

    '''
    {
        "name": "test.txt",
        "operation": "read",
        "mode": "none"
        "offset": 1,
        "length": 10,
        "contents": "",
    }
    '''

    # 使用json.loads()函数解析JSON字符串
    json_string = data
    parsed_data = json.loads(json_string)

    # 打印解析后的Python对象
    if debug_mode:
        print("<<<", end=" ")
        print(parsed_data)

    operation = parsed_data["operation"]    

    try:
        if operation == "fopen":
            name = parsed_data["name"]
            mode = parsed_data["mode"]    
            file_fd = open(name, mode)
            rsp_content = "{\"status\": \"ok\"}"

        elif operation == "fseek":
            offset = parsed_data["offset"]
            whence = parsed_data["whence"]
            file_fd.seek(offset, whence)
            rsp_content = "{\"status\": \"ok\"}"

        elif operation == "ftell":
            offset = file_fd.tell()
            rsp_content = "{\"status\": \"ok\", \"offset\": " + str(offset) + "}"

        elif operation == "fread":
            length = parsed_data["length"]
            contents_rd_byte_array = file_fd.read(length)
            # 使用binascii.hexlify()将字节数组转换为十六进制字符串
            contents_hex_string = binascii.hexlify(contents_rd_byte_array).decode('utf-8')
            rsp_content = "{\"status\": \"ok\", \"contents\": \"" +  contents_hex_string + "\"}"

        elif operation == "fwrite":
            length = parsed_data["length"]
            contents = parsed_data["contents"]
            contents_wr_byte_array = binascii.unhexlify(contents)
            file_fd.write(contents_wr_byte_array)
            rsp_content = "{\"status\": \"ok\"}"

        elif operation == "fclose":
            file_fd.close()
            rsp_content = "{\"status\": \"ok\"}"
        
        else:
            rsp_content = "{\"status\": \"unknown operation\"}"

        if debug_mode:
            print(">>> " + rsp_content)
        
    except Exception as e:
        rsp_content = str(e)

    if tcp_mode:
        connection.sendall(rsp_content.encode('utf-8'))
    else:
        serverSock.sendto(rsp_content.encode('utf-8'), addr)
    if debug_mode:
        print("")
