import socket
import sys
import json
import socket
import os
import shutil
import binascii
import time

import base64
import urllib
import requests
import json
import os

import urllib
import urllib.response
import urllib.request
import urllib.parse
import http.cookiejar

debug_mode = False

API_KEY = "KgRRbuj3824giWkMOP5tQfON"
SECRET_KEY = "hs2De8mXboVmznrQcl9paqTOSURojNfd"

def speech_decode_by_pcm_online(audio_data_base64, audio_file_size):

    #print(len(audio_data_base64))
    #print(audio_file_size)

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
    #print(response)
    result = response.json()
    #print(result)

    if result['err_msg'] is not None:
        ret_result = result['err_msg']
        if ret_result == 'success.':
            ret_result = result['result'][0]

    if debug_mode:
        #print(response.text)
        print(ret_result)

    return ret_result
 
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

import requests

def get_access_token2():
    """
    使用 AK，SK 生成鉴权签名（Access Token）
    :return: access_token，或是None(如果错误)
    """
    url = "https://aip.baidubce.com/oauth/2.0/token"
    params = {"grant_type": "client_credentials", "client_id": API_KEY, "client_secret": SECRET_KEY}
    return str(requests.post(url, params=params).json().get("access_token"))

def baidu_tts(text_req, audio_file):
        
    url = "https://tsn.baidu.com/text2audio"
    
    payload='tex=' + '1234567890' + '&tok='+ get_access_token2() +'&cuid=DfV47ziDDbsSE6a5nUarA4BDZGuXzrQB&ctp=1&lan=zh&spd=5&pit=5&vol=5&per=1&aue=4'
    headers = {
        'Content-Type': 'application/x-www-form-urlencoded',
        'Accept': '*/*'
    }
    
    print(payload)

    response = requests.request("POST", url, headers=headers, data=payload)
    print(response)

    if response:
        with open(audio_file, 'w') as f:
            f.write(response.text)
    
    #print(response.text)

def baidu_tts_plus_query(task_id):
    url = "https://aip.baidubce.com/rpc/2.0/tts/v1/query?access_token=" + get_access_token()
    
    payload = json.dumps({
        "task_ids": [
            task_id
        ]
    })
    headers = {
        'Content-Type': 'application/json',
        'Accept': 'application/json'
    }
    
    response = requests.request("POST", url, headers=headers, data=payload)    
    
    if debug_mode:
        print(response.encoding)
        print(response.text)

    rsp_json = response.json()
    task_status = rsp_json['tasks_info'][0]['task_status']
    if task_status == 'Success':
        speech_url = rsp_json['tasks_info'][0]['task_result']['speech_url']
        return speech_url
    else:
        return None

def baidu_tts_plus(text_req, audio_file):
        
    url = "https://aip.baidubce.com/rpc/2.0/tts/v1/create?access_token=" + get_access_token2()
    
    payload = json.dumps({
        "text": text_req,
        "format": "mp3-16k",
        "voice": 0,
        "lang": "zh",
        "speed": 5,
        "pitch": 5,
        "volume": 5,
        "enable_subtitle": 0
    })
    #print(payload)
    headers = {
        'Content-Type': 'application/json',
        'Accept': 'application/json'
    }
    
    response = requests.request("POST", url, headers=headers, data=payload)
    
    #print(response.text)

    rsp_json = response.json()
    task_id = rsp_json['task_id']
    #print(task_id)

    while True:
        url = baidu_tts_plus_query(task_id)
        #print(url)
        if url is not None:
            try:
                #print('download url file ...')
                f = urllib.request.urlopen(url)
                data = f.read()
                with open(audio_file, "wb") as code:
                  code.write(data)
                #print('download url file ... ok')
                break
            except Exception as e:
                print(e)
        time.sleep(1)

    return url

################################################################3

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
                rsp_content = "{\"status\": \"ok\", \"speech_rsp\": \"" + text_result + "\"}"        
        elif operation == "kimi_ai":
            text_req = parsed_data["text_req"]
            text_rsp = ai_get_answer_from_kimi(text_req)            
            rsp_content = "{\"status\": \"ok\", \"text_rsp\": \"" + text_rsp + "\"}"
        elif operation == "baidu_tts":
            text_req = parsed_data["text_req"]
            audio_file = parsed_data["audio_file"]
            #print('---===== ' + audio_file)
            #baidu_tts(text_req, audio_file)
            speech_url = baidu_tts_plus(text_req, audio_file)
            rsp_content = "{\"status\": \"ok\", \"speech_url\": \"" + speech_url + "\"}"
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
