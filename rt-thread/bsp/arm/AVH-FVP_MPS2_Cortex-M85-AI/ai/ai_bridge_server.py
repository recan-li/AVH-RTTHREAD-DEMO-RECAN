import socket
import sys
import json
import socket
import os
import shutil
import binascii

import base64
import urllib
import requests
import json
import os

API_KEY = "KgRRbuj3824giWkMOP5tQfON"
SECRET_KEY = "hs2De8mXboVmznrQcl9paqTOSURojNfd"

def speech_decode_by_pcm_online(audio_data_base64, audio_file_size):

    print(len(audio_data_base64))
    print(audio_file_size)

    url = "https://vop.baidu.com/server_api"
    
    # speech 可以通过 get_file_content_as_base64("C:\fakepath\test.pcm",False) 方法获取
    payload = json.dumps({
        "format": "pcm",
        "rate": 16000,
        "channel": 1,
        "cuid": "P4Dz9FMQiJpA47ztMBc4QdWsBFeAhW55",
        "token": get_access_token(),
        "speech": audio_data_base64,
        "len": audio_file_size
    })

    headers = {
        'Content-Type': 'application/json',
        'Accept': 'application/json'
    }

    response = requests.request("POST", url, headers=headers, data=payload)    
    result = response.json()

    if debug_mode:
        #print(response.text)
        print(result['result'][0])

    return result['result'][0]
 
def speech_decode_by_file_online(audio_file):
    # 读取语音文件并转换为base64格式
    #with open(audio_file, 'rb') as f:
    #    audio_data_base64 = base64.b64encode(f.read()).decode()

    with open('1.txt', 'w') as f:
        f.write(audio_data_base64)

    speech_decode_by_pcm_online(audio_data_base64, os.stat(audio_file).st_size)

def get_file_content_as_base64(path, urlencoded=False):
    """
    获取文件base64编码
    :param path: 文件路径
    :param urlencoded: 是否对结果进行urlencoded 
    :return: base64编码信息
    """
    with open(path, "rb") as f:
        content = base64.b64encode(f.read()).decode("utf8")
        if urlencoded:
            content = urllib.parse.quote_plus(content)
    return content

def get_access_token():
    """
    使用 AK，SK 生成鉴权签名（Access Token）
    :return: access_token，或是None(如果错误)
    """
    url = "https://aip.baidubce.com/oauth/2.0/token"
    params = {"grant_type": "client_credentials", "client_id": API_KEY, "client_secret": SECRET_KEY}
    return str(requests.post(url, params=params).json().get("access_token"))

#if __name__ == '__main__':
#    speech_decode_by_file_online('./test.wav')
#    speech_decode_by_file_online('./test1.raw')

import openai
import json
import requests
from openai import OpenAI

client = OpenAI(
    api_key="sk-52OQjRvWkkJrSor3V4W3ZeVLz6ArQyHED1RP4xiBS2KF5mdA",
    base_url="https://api.moonshot.cn/v1",
)

def ai_get_answer_from_kimi(message):
    try:
        #print('connecting to kimi ...')
        completion = client.chat.completions.create(
            model="moonshot-v1-8k",
            messages=[
                {
                    "role": "user",
                    "content": message
                },
            ],
            temperature=0.3,
        )
         
        answer = completion.choices[0].message.content
        if debug_mode:
            print("*" * 30)
            print(answer)
        return answer
    except Exception as e:
        print(e)
        return "Error response now !"

##############################################################33

debug_mode = True

tcp_mode = False

# 定义服务器的IP地址和端口号
SERVER_IP_ADDRESS = "127.0.0.1"
if tcp_mode:
    TCP_PORT_NO = 55557
else:
    UDP_PORT_NO = 55558

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

base64_data = ""

while True:
    if tcp_mode:
        data = connection.recv(102400)
    else:
        data, addr = serverSock.recvfrom(102400)

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
        if operation == "baidu_stt":
            cur_part = parsed_data["cur_part"]
            max_part = parsed_data["max_part"]
            if cur_part == 1:
                # clear base64 data
                base64_data = ""
            if cur_part != max_part:
                base64_data += parsed_data["base64"]
                rsp_content = "{\"status\": \"ok\"}"
            elif cur_part == max_part:
                base64_data += parsed_data["base64"]
                file_len = parsed_data["file_len"]
                #speech_decode_by_file_online('./test.wav')
                #with open('2.txt', 'w') as f:
                #    f.write(base64_data)
                text_result = speech_decode_by_pcm_online(base64_data, file_len)
                text_answer = ai_get_answer_from_kimi(text_result)
                rsp_content = "{\"status\": \"ok\", \"text_req\": \"" + text_result + "\", \"text_rsp\": \"" + text_answer + "\"}"
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
