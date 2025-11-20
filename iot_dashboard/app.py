import os
from flask import Flask, render_template, request, redirect, url_for, flash, jsonify # <--- THÊM jsonify
from flask_login import LoginManager, login_user, login_required, logout_user, current_user
from models import db, User
from werkzeug.security import generate_password_hash, check_password_hash

# --- CẤU HÌNH ĐƯỜNG DẪN ---
BASE_DIR = os.path.dirname(os.path.abspath(__file__))
LOG_FILE = os.path.join(BASE_DIR, 'data', 'log.txt')
DB_FILE = os.path.join(BASE_DIR, 'instance', 'users.db')

app = Flask(__name__)
app.secret_key = "your_secret_key"
app.config['SQLALCHEMY_DATABASE_URI'] = f'sqlite:///{DB_FILE}'
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False

db.init_app(app)

login_manager = LoginManager()
login_manager.init_app(app)
login_manager.login_view = 'login'

@login_manager.user_loader
def load_user(user_id):
    return User.query.get(int(user_id))

with app.app_context():
    db.create_all()

# --- HÀM ĐỌC DỮ LIỆU LOG ---
# --- HÀM ĐỌC DỮ LIỆU LOG (ĐÃ CẬP NHẬT CHO BIOMETRIC & ICP-10101) ---
def read_data():
    data = []
    if not os.path.exists(LOG_FILE):
        return data 
    try:
        with open(LOG_FILE, "r", encoding="utf-8") as f:
            lines = f.readlines()
            for line in reversed(lines):
                line = line.strip()
                if not line or ']' not in line: continue
                
                try:
                    # 1. Tách thời gian và nội dung
                    parts = line.split('] ', 1)
                    time_str = parts[0].replace('[', '')
                    content = parts[1] # Ví dụ: dev=ck sensor=icp10101 temp=30C press=1000Pa
                    
                    # 2. Chuẩn bị biến lưu trữ
                    item = {
                        'time': time_str,
                        'sensor': 'Unknown',
                        'info': '' # Chúng ta sẽ gom hết thông số vào chuỗi này
                    }
                    
                    # 3. Tách các từ khóa (temp=..., hr=...)
                    tokens = content.split(' ')
                    details = [] # Danh sách tạm để ghép chuỗi hiển thị
                    
                    for token in tokens:
                        if '=' not in token: continue
                        
                        key, value = token.split('=', 1)
                        
                        # --- BẮT CÁC TRƯỜNG DỮ LIỆU ---
                        if key == 'sensor':
                            item['sensor'] = value.upper() # Viết hoa tên sensor cho đẹp
                        
                        # Cảm biến ICP-10101
                        elif key == 'temp':
                            details.append(f"Nhiệt độ: {value}")
                        elif key == 'press':
                            details.append(f"Áp suất: {value}")
                        elif key == 'alt':
                            details.append(f"Độ cao: {value}")
                            
                        # Cảm biến Biometric
                        elif key == 'hr': # Heart Rate
                            details.append(f"Nhịp tim: {value}")
                        elif key == 'spo2':
                            details.append(f"SpO2: {value}")
                            
                        # Cảm biến cũ (ZMOD) - Giữ lại để không lỗi code cũ
                        elif key == 'tvoc':
                            details.append(f"TVOC: {value}")
                        elif key == 'iaq':
                            details.append(f"IAQ: {value}")

                    # Ghép các thông số lại thành 1 dòng string đẹp (VD: "Nhiệt độ: 30C | Áp suất: 100Pa")
                    item['info'] = " | ".join(details)
                    
                    data.append(item)
                except Exception:
                    continue
    except Exception:
        pass
    return data

# --- CÁC ROUTE ---

@app.route("/login", methods=["GET", "POST"])
def login():
    if request.method == "POST":
        username = request.form["username"]
        password = request.form["password"]
        user = User.query.filter_by(username=username).first()
        if user and check_password_hash(user.password, password):
            login_user(user)
            return redirect(url_for("dashboard"))
        else:
            return render_template("login.html", error="Sai tên đăng nhập hoặc mật khẩu")
    return render_template("login.html")

@app.route("/register", methods=['GET', 'POST'])
def register():
    if request.method == 'POST':
        username = request.form['username']
        password = request.form['password']
        existing_user = User.query.filter_by(username=username).first()
        if existing_user:
            return render_template('register.html', error="Tài khoản đã tồn tại")
        
        hash_password = generate_password_hash(password) 
        new_user = User(username=username, password=hash_password)
        db.session.add(new_user)
        db.session.commit()
        return redirect(url_for('login'))   
    return render_template('register.html')

@app.route("/logout")
@login_required
def logout():
    logout_user()
    return redirect(url_for("login"))

@app.route("/")
@login_required
def dashboard():
    # Lúc mới vào trang vẫn load dữ liệu lần đầu
    sensor_data = read_data()
    return render_template("dashboard.html", data=sensor_data, username=current_user.username)

@app.route('/clear_log', methods=['POST'])
@login_required
def clear_log():
    try:
        open(LOG_FILE, 'w').close()
    except Exception:
        pass
    return redirect(url_for('dashboard'))

# --- API TRẢ DỮ LIỆU JSON (MỚI) ---
# Đây là cái giúp trang web cập nhật ngầm
@app.route('/api/get_data')
@login_required
def get_data_json():
    data = read_data()
    return jsonify(data)

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000, debug=True)