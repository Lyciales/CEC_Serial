import csv
import random
import os

# Đường dẫn lưu file
filename = r"C:\Users\Lyciales\Documents\UniversityDocuments\CapsuleEndoscopeControl\CEC_Serial\.gitignore\test data\test_cases.csv"

# Tạo thư mục nếu chưa tồn tại
os.makedirs(os.path.dirname(filename), exist_ok=True)

with open(filename, mode='w', newline='') as file:
    writer = csv.writer(file)
    # Ghi Header
    # writer.writerow(["Address", "State", "Current"])
    
    for _ in range(10000):
        # Sinh giá trị ngẫu nhiên
        address = random.choice([1, 2])
        state = random.choice([0, 1])  # 0: Off, 1: On
        current = random.uniform(-20.0, 20.0) # Không cần dùng round() nữa vì format sẽ tự làm tròn
        
        # Ép định dạng tất cả về chuỗi có 3 chữ số thập phân (.3f)
        address_fmt = f"{address:.2f}"
        state_fmt = f"{state:.2f}"
        current_fmt = f"{current:.2f}"
        
        # Ghi vào file
        writer.writerow([address_fmt, state_fmt, current_fmt])

print(f"Đã tạo thành công 10000 test cases (định dạng .3f) tại:\n{filename}")