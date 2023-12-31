 #!/usr/bin/env python3
"""Proxy Server with additional functionalities."""
from socket import AF_INET, socket, SOCK_STREAM, timeout
from threading import Thread
import datetime
import os
import threading
import time
# Hàm để kiểm tra và xóa hình ảnh cũ khỏi cache
def clean_image_cache(image_cache):
    while True:
        now = datetime.datetime.now()

        # Loop through each domain-specific cache folder
        for domain in os.listdir("cache"):
            domain_cache_dir = os.path.join("cache", domain)

            # Check if the domain_cache_dir is a directory
            if not os.path.isdir(domain_cache_dir):
                continue

            # Get a list of images in the cache folder along with their expiration times
            '''images_in_cache = os.listdir(domain_cache_dir)
            image_cache_info = {}
            for image_file in images_in_cache:
                image_path = os.path.join(domain_cache_dir, image_file)
                expiration_time = datetime.datetime.fromtimestamp(os.path.getmtime(image_path)) + datetime.timedelta(seconds=cache_expiration)
                image_cache_info[image_file] = {"path": image_path, "expiration": expiration_time}

            # Remove the images from the cache whose expiration time has passed
            keys_to_delete = [key for key, value in image_cache_info.items() if value["expiration"] < now]
            for key in keys_to_delete:
                # Remove the image data from the cache
                os.remove(image_cache_info[key]["path"])
                print(f"Xóa hình ảnh cũ khỏi cache - URL: {key}")'''

            cache_folder_mod_time = datetime.datetime.fromtimestamp(os.path.getmtime(domain_cache_dir))
            if cache_folder_mod_time + datetime.timedelta(seconds=cache_expiration) < now:
                # Remove the entire cache folder for the expired website
                for root, dirs, files in os.walk(domain_cache_dir, topdown=False):
                    for file in files:
                        file_path = os.path.join(root, file)
                        os.remove(file_path)
                        print(f"Xóa hình ảnh cũ khỏi cache - Path: {file_path}")
                    os.rmdir(root)  # Remove the subdirectories

                print(f"Đã xóa thư mục cache hết hạn - Domain: {domain}")
        # Đợi 1 phút trước khi kiểm tra và xóa lại
        time.sleep(10)


# Tạo và khởi chạy một luồng đồng hồ
clean_thread = threading.Thread(target=clean_image_cache)
clean_thread.start()
# Kiểm tra và tạo thư mục cache nếu chưa tồn tại
if not os.path.exists("cache"):
    os.makedirs("cache")


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
def is_whitelisted(url, whitelist):
    url_parts = url.split("/")
    if len(url_parts) >= 3:
        domain = url_parts[2].lower()
        for allowed_domain in whitelist:
            if domain.endswith(allowed_domain.lower()):
                return True
    return False




#C:\Users\Admin\Desktop\network\proxy_1.py
# Check if it's within valid time (8AM to 8PM)
def is_valid_time(start_time, end_time):
    now = datetime.datetime.now()
    current_hour = now.hour
    return start_time <= current_hour <= end_time

# Đọc cấu hình từ tệp config.txt
def read_config():
    config = {}
    try:
        with open("config.txt") as f:
            for line in f:
                key, value = line.strip().split("=")
                if key == "WHITELIST":
                    config[key] = [domain.strip() for domain in value.split(",") if domain.strip()]
                elif key == "CACHE_EXPIRATION":
                    config[key] = int(value)
                elif key == "START_TIME":
                    config[key] = int(value)
                elif key == "END_TIME":
                    config[key] = int(value)
                else:
                    config[key] = value
    except FileNotFoundError:
        pass
    return config


def accept_incoming_connections():
    """Sets up handling for incoming clients."""
    while True:
        client, client_address = SERVER.accept()
        print("%s:%s has connected." % client_address)
        addresses[client] = client_address
        Thread(target=handle_client, args=(client, config["START_TIME"], config["END_TIME"])).start()

def handle_client(client, start_time, end_time):
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
        if not is_valid_time(start_time, end_time):
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
                # Đặt thời gian chờ nhận dữ liệu là 1 giây
                server_socket.settimeout(1)

                try:
                    response_data = server_socket.recv(4096)
                    if len(response_data) > 0:
                        client.send(response_data)
                        #print(2)
                    else:
                        break
                except timeout:
                    #print("Timeout-------")
                    # Khi thời gian chờ hết, thoát khỏi vòng lặp
                    break
            server_socket.close()
        except Exception as e:
            print(f"Error: {e}")

# Close the client connection if an error occurs
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

# Hàm để lấy giá trị băm MD5 của một URL
def get_url_hash(url):
    return hashlib.md5(url.encode()).hexdigest()

# Hàm để lấy hình ảnh từ máy chủ web
'''def fetch_image_from_webserver(url):
    try:
        url_parts = url.split("://", 1)
        if len(url_parts) == 2:
            protocol, remaining = url_parts
        else:
            print("URL không hợp lệ.")
            return None
        
        domain_end = remaining.find("/")
        if domain_end == -1:
            domain = remaining
            path = "/"
        else:
            domain = remaining[:domain_end]
            path = remaining[domain_end:]
        
        # Create a socket to connect to the web server
        server_socket = socket(AF_INET, SOCK_STREAM)
        server_socket.connect((domain, 80))
        
        # Send the HTTP request manually
        request = (
            f"GET {path} HTTP/1.1\r\n"
            f"Host: {domain}\r\n"
            f"Connection: close\r\n"
            f"\r\n"
        )
        server_socket.sendall(request.encode())
        
        response = b""
        while True:
            chunk = server_socket.recv(4096)
            if not chunk:
                break
            response += chunk
        
        server_socket.close()
        
        # Find the start of the image data
        image_start = response.find(b'\r\n\r\n') + 4
        image_data = response[image_start:]
        
        if image_data:
            # Lấy giá trị băm của URL để dùng làm khóa cache
            image_key = get_url_hash(url)

            # Lấy domain name từ URL để tạo thư mục cache tương ứng
            domain = domain.lower()

            # Tạo thư mục cache nếu chưa tồn tại
            cache_dir = os.path.join("cache", domain)
            if not os.path.exists(cache_dir):
                os.makedirs(cache_dir)

            # Tạo đường dẫn tới file hình ảnh trong thư mục cache
            file_path = os.path.join(cache_dir, f"{image_key}.jpg")

            # Lưu hình ảnh vào thư mục cache
            with open(file_path, "wb") as f:
                f.write(image_data)

            return image_data
        else:
            print("Lỗi khi tải hình ảnh từ máy chủ web - Không có dữ liệu hình ảnh")
            return None
    except Exception as e:
        print(f"Lỗi khi tải hình ảnh từ máy chủ web: {e}")
        return None
'''
def fetch_image_from_webserver(url):
    try:
        # Parse URL manually
        scheme, netloc, path, params, query, fragment = "", "", "", "", "", ""
        parts = url.split("://")
        if len(parts) > 1:
            scheme = parts[0]
            remaining = parts[1]
            if "/" in remaining:
                netloc, path = remaining.split("/", 1)
                path = "/" + path
            else:
                netloc = remaining
        else:
            netloc = parts[0]

        webserver = netloc
        path = path or '/'

        request_data = f"GET {path} HTTP/1.1\r\nHost: {webserver}\r\nConnection: close\r\n\r\n"

        port = 80 if scheme == "http" else 443  # Adjust port based on scheme

        server_sock = socket(AF_INET, SOCK_STREAM)
        server_sock.connect((webserver, port))
        server_sock.sendall(request_data.encode())

        image_data = b""
        while True:
            response_data = server_sock.recv(BUFSIZ)
            if len(response_data) > 0:
                image_data += response_data
            else:
                break

        server_sock.close()

        image_key = get_url_hash(url)
        domain = netloc.lower()

        cache_dir = os.path.join("cache", domain)
        if not os.path.exists(cache_dir):
            os.makedirs(cache_dir)

        file_path = os.path.join(cache_dir, f"{image_key}.jpg")
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
            #print(value["expiration"], " -----" , datetime.datetime.now())
            # Xóa hình ảnh cũ khỏi cache nếu đã hết hạn
            for key, value in list(image_cache.items()):
                if value["expiration"] < datetime.datetime.now():
                    del image_cache[key]
                    print(f"Xóa hình ảnh cũ khỏi cache - URL: {key}")
            
            return image_data

    return None


def broadcast(msg, prefix=""):  # prefix is for name identification.
    """Broadcasts a message to all the clients."""

    for sock in clients:
        sock.send(bytes(prefix, "utf8")+msg)

        
clients = {}
addresses = {}

HOST = '172.29.65.188'
PORT =8888
BUFSIZ = 4096
ADDR = (HOST, PORT)

SERVER = socket(AF_INET, SOCK_STREAM)
SERVER.bind(ADDR)


if __name__ == "__main__":
    config = read_config()
    cache_expiration = int(config.get("CACHE_EXPIRATION", config["CACHE_EXPIRATION"]))
    print(cache_expiration, " -----")
    whitelist = config.get("WHITELIST", config["WHITELIST"])
    clean_thread = threading.Thread(target=clean_image_cache, args=(image_cache,))
    clean_thread.start()
    for domain in whitelist :
        print("domain:    ",domain)
    SERVER.listen(5)
    print("Waiting for connection...")
    ACCEPT_THREAD = Thread(target=accept_incoming_connections)
    ACCEPT_THREAD.start()
    ACCEPT_THREAD.join()
    SERVER.close()
