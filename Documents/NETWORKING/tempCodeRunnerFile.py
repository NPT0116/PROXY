#!/usr/bin/env python3
"""Proxy Server with additional functionalities."""
from socket import AF_INET, socket, SOCK_STREAM
from threading import Thread
import datetime
# Define the cache expiration time in seconds (15 minutes)
DEFAULT_CACHE_EXPIRATION = 900
import os

# Kiểm tra và tạo thư mục cache nếu chưa tồn tại
if not os.path.exists("cache"):
    os.makedirs("cache")

# Define the default whitelist (allow access to *.example.com/*)
DEFAULT_WHITELIST = ["example.com", "oosc.online","vbsca.ca/login/login.asp"]

# Read configuration from file (if available)
def read_config():
    config = {}
    try:
        with open("config.txt") as f:
            for line in f:
                key, value = line.strip().split("=")
                if key == "WHITELIST":
                    config[key] = [domain.strip() for domain in value.split(",") if domain.strip()]  # Split the value into a list
                elif key == "CACHE_EXPIRATION":
                    config[key] = int(value)
                elif key in ["START_TIME", "END_TIME"]:
                    config[key] = int(value)
                else:
                    config[key] = value
    except FileNotFoundError:
        pass
    return config



# Check if the URL is whitelisted
from urllib.parse import urlparse

# Check if the URL is whitelisted
def is_whitelisted(url, whitelist):
    parsed_url = urlparse(url)
    domain = parsed_url.netloc.lower()


    for allowed_domain in whitelist:
        print("Domain:", allowed_domain, "   " , domain)
        if domain.endswith(allowed_domain.lower()):
            return True

    return False




#C:\Users\Admin\Desktop\network\proxy_1.py
# Check if it's within valid time (8AM to 8PM)
def is_valid_time():
    now = datetime.datetime.now()
    current_hour = now.hour
    print("Current hour:", current_hour)
    return 8 <= current_hour <= 20


def accept_incoming_connections():
    """Sets up handling for incoming clients."""
    while True:
        client, client_address = SERVER.accept()
        print("%s:%s has connected." % client_address)
        addresses[client] = client_address
        Thread(target=handle_client, args=(client,)).start()

def handle_client(client):
    """Handles a single client connection."""
    request_data = client.recv(4096)
    first_line = request_data.decode().split('\n')[0]

    # Check if it's an HTTPS CONNECT request
    if first_line.startswith("CONNECT"):
        print("Received an HTTPS CONNECT request. Ignoring...")
        client.close()
        return

    method = first_line.split(' ')[0]
    url = first_line.split(' ')[1]

    # Print the request data
    print("---------- HTTP REQUEST ----------")
    print(request_data.decode())
    print("----------------------------------")
    # Trong hàm handle_client()
    if method == "GET" and url.endswith((".jpg", ".jpeg", ".png", ".gif")):
        image_data = handle_image_request(url)
        if image_data:
            # Gửi dữ liệu hình ảnh cho client
            client.send(image_data)
        else:
            # Không tìm thấy hình ảnh hoặc lỗi khi tải, trả về mã phản hồi 404 Not Found
            response = "HTTP/1.0 404 Not Found\r\nContent-Type: text/html\r\n\r\n<h1>404 Not Found</h1>"
            client.send(response.encode())
        client.close()
        return

    # Check if the method is supported (GET, POST, HEAD)
    if method in ["GET", "POST", "HEAD"]:
        # Check if the URL is whitelisted
        
        if not is_whitelisted(url, whitelist):
            print("Forbidden access (not whitelisted) - URL:", url)
            response = "HTTP/1.0 403 Forbidden\r\nContent-Type: text/html\r\n\r\n<h1>403 Forbidden</h1>"
            client.send(response.encode())
            client.close()
            return

        # Check if it's within valid time
        if not is_valid_time():
            print("Forbidden access (out of time) - URL:", url)
            response = "HTTP/1.0 403 Forbidden\r\nContent-Type: text/html\r\n\r\n<h1>403 Forbidden</h1>"
            client.send(response.encode())
            client.close()
            return

        # Rest of the code for handling HTTP requests remains the same...
        http_pos = url.find("://")
        if http_pos == -1:
            temp = url
        else:
            temp = url[(http_pos + 3):]

            port_pos = temp.find(":")
            webserver_pos = temp.find("/")
        if webserver_pos == -1:
            webserver_pos = len(temp)
            webserver = ""
            port = -1
        if port_pos == -1 or webserver_pos < port_pos:
            port = 80
            webserver = temp[:webserver_pos]
        else:
            port = int((temp[(port_pos + 1):])[:webserver_pos - port_pos - 1])
            webserver = temp[:port_pos]
        # ...

        try:
            # Create a socket to connect to the web server
            server_socket = socket(AF_INET, SOCK_STREAM)
            server_socket.connect((webserver, port))
            server_socket.sendall(request_data)

            while True:
                response_data = server_socket.recv(4096)
                if len(response_data) > 0:
                    client.send(response_data)
                else:
                    break
        except Exception as e:
            print(f"Error: {e}")

        client.close()
    else:
        # Handle unsupported methods (e.g., PUT, PATCH, DELETE, OPTIONS)
        response = "HTTP/1.0 403 Forbidden\r\nContent-Type: text/html\r\n\r\n<h1>403 Forbidden</h1>"
        client.send(response.encode())
        print("Forbidden access (unsupported method)")

    client.close()

# Define the cache for storing images and their expiration times
image_cache = {}

import os
import hashlib
import urllib.request

# Hàm để lấy giá trị băm MD5 của một URL
def get_url_hash(url):
    return hashlib.md5(url.encode()).hexdigest()

# Hàm để lấy hình ảnh từ máy chủ web
def fetch_image_from_webserver(url):
    try:
        with urllib.request.urlopen(url) as response:
            image_data = response.read()

            # Lấy giá trị băm của URL để dùng làm tên file
            image_key = get_url_hash(url)
            # print("IMG key: ", image_key)
            file_path = os.path.join("cache", f"{image_key}.jpg")
            # print("PATH: ", file_path)
            # Lưu hình ảnh vào thư mục cache
            with open(file_path, "wb") as f:
                f.write(image_data)

            return image_data
    except Exception as e:
        print(f"Lỗi khi tải hình ảnh từ máy chủ web: {e}")
        return None

def handle_image_request(url):
    """Xử lý yêu cầu hình ảnh, phục vụ từ bộ nhớ cache hoặc tải từ máy chủ web."""
    global image_cache, cache_expiration

    # Lấy giá trị băm của URL để dùng làm khóa cache
    image_key = get_url_hash(url)

    # Kiểm tra nếu hình ảnh có trong bộ nhớ cache và chưa hết hạn
    if image_key in image_cache and image_cache[image_key]["expiration"] >= datetime.datetime.now():
        print(f"Phục vụ hình ảnh từ bộ nhớ cache - URL: {url}")
        return image_cache[image_key]["data"]
    else:
        print(f"Tải hình ảnh từ máy chủ web - URL: {url}")
        image_data = fetch_image_from_webserver(url)

        # Nếu hình ảnh tải thành công, lưu vào bộ nhớ cache với thời gian hết hạn
        if image_data:
            expiration_time = datetime.datetime.now() + datetime.timedelta(seconds=cache_expiration)
            image_cache[image_key] = {"data": image_data, "expiration": expiration_time}
            return image_data

    return None


def broadcast(msg, prefix=""):  # prefix is for name identification.
    """Broadcasts a message to all the clients."""

    for sock in clients:
        sock.send(bytes(prefix, "utf8")+msg)

        
clients = {}
addresses = {}

HOST = '192.168.4.15'
PORT =8888
BUFSIZ = 4096
ADDR = (HOST, PORT)

SERVER = socket(AF_INET, SOCK_STREAM)
SERVER.bind(ADDR)

if __name__ == "__main__":
    config = read_config()
    cache_expiration = int(config.get("CACHE_EXPIRATION", DEFAULT_CACHE_EXPIRATION))
    whitelist = config.get("WHITELIST", DEFAULT_WHITELIST)
    for domain in whitelist :
        print("domain:    ",domain)
    SERVER.listen(5)
    print("Waiting for connection...")
    ACCEPT_THREAD = Thread(target=accept_incoming_connections)
    ACCEPT_THREAD.start()
    ACCEPT_THREAD.join()
    SERVER.close()
