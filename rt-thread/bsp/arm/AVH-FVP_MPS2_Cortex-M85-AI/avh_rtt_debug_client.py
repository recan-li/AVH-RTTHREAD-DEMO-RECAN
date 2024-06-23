import hashlib
import sys
import json
import socket
import os
import shutil

# 服务器地址和端口号
SERVER_HOST = '127.0.0.1'
SERVER_PORT0 = 12345
SERVER_PORT1 = 12346

def avh_rtt_debug_loop():
	received_data = None
	client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

	try:
		print("Conncting debug server ...", end=" ")
		client_socket.connect((SERVER_HOST, SERVER_PORT0))

	except Exception as e:
		print("Catch exception:", e)
		client_socket.close()
		try:
			print("Conncting debug server ...", end=" ")
			client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
			client_socket.connect((SERVER_HOST, SERVER_PORT1))

		except Exception as e:
			print("Catch exception:", e)
			return None

	client_socket.settimeout(0.5)

	try:
		print("Conncted debug server ok ...")
		while True:			
			while True:
				print("msh >", end=' ')
				user_input = input("")
				if user_input != "":
					break
			client_socket.sendall(user_input.encode())
			
			received_data = b""
			cnt = 0
			while  True:
				try:
					while True:				
						part_data = client_socket.recv(4096)
						if part_data is not None:
							received_data += part_data
						else:
							print("none")
							break				
				except socket.timeout:
					#print(len(received_data))
					if len(received_data) == 0:
						cnt = cnt + 1
						if cnt > 10:
							break
						else:
							continue
					else:
						break
						print("", end='')

			print(received_data.decode())
	
	except Exception as e:
		print("Catch exception:", e)
		return None

	finally:
		client_socket.close()
		return received_data

def help():
	print("Usage: python " + sys.argv[0] + " [debug]")

if __name__ == "__main__":
	if len(sys.argv) < 2:
		help()
		sys.exit(1)

	if len(sys.argv) > 2:
		SERVER_PORT = int(sys.argv[2])

	operation = sys.argv[1]
	if operation == "debug":		
		avh_rtt_debug_loop()
	else:
		help()
		sys.exit(1)

	sys.exit(0)
