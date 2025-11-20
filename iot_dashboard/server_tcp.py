import socket
import datetime
import os

# --- CẤU HÌNH ĐƯỜNG DẪN TỰ ĐỘNG ---
# 1. Lấy đường dẫn của file server_tcp.py hiện tại
BASE_DIR = os.path.dirname(os.path.abspath(__file__))

# 2. Nối đường dẫn vào thư mục data/log.txt
# Kết quả sẽ tương tự: /home/nguyen/.../IOT_DASHBOARD/data/log.txt
LOG_FILE = os.path.join(BASE_DIR, 'data', 'log.txt')

# --- CẤU HÌNH MẠNG ---
HOST = '0.0.0.0'
PORT = 65432

def save_to_log(data):
    """Hàm này lưu dữ liệu vào file log.txt trong thư mục data"""
    try:
        # Mở file log để ghi (mode 'a' là nối tiếp)
        with open(LOG_FILE, "a", encoding="utf-8") as f:
            f.write(data + "\n")
    except Exception as e:
        print(f"Lỗi khi lưu file tại {LOG_FILE}: {e}")

def start_server():
    print(f"--- SERVER TCP ĐANG CHẠY ---")
    print(f"Server IP: {HOST} | Port: {PORT}")
    print(f"Đang ghi log vào: {LOG_FILE}")
    
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    
    try:
        server_socket.bind((HOST, PORT))
        server_socket.listen(5)
        print("Đang chờ kết nối từ Kit...")

        while True:
            conn, addr = server_socket.accept()
            print(f"-> Kết nối mới từ: {addr}")

            with conn:
                while True:
                    data = conn.recv(1024)
                    if not data:
                        break
                    
                    # Xử lý dữ liệu nhận được
                    message = data.decode('utf-8').strip()
                    current_time = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
                    log_line = f"[{current_time}] {message}"
                    
                    # 1. In ra màn hình
                    print(log_line)
                    # 2. Lưu vào file
                    save_to_log(log_line)
            
            print(f"Ngắt kết nối với {addr}. Tiếp tục chờ...")

    except KeyboardInterrupt:
        print("\nServer đã dừng.")
    finally:
        server_socket.close()

if __name__ == "__main__":
    start_server()