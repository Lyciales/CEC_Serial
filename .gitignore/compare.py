import csv

def compare_csv_files(file1_path, file2_path):
    print(f"Đang so sánh:\n1. {file1_path}\n2. {file2_path}\n{'-'*50}")
    
    with open(file1_path, mode='r', encoding='utf-8') as f1, \
         open(file2_path, mode='r', encoding='utf-8') as f2:
        
        reader1 = csv.reader(f1)
        reader2 = csv.reader(f2)
        
        # Đọc toàn bộ dữ liệu vào list
        data1 = list(reader1)
        data2 = list(reader2)

    total_rows_1 = len(data1)
    total_rows_2 = len(data2)
    
    # 1. Kiểm tra số lượng bản tin (Phát hiện lỗi mất gói/rớt frame)
    if total_rows_1 != total_rows_2:
        print(f"[CẢNH BÁO] Số lượng dòng không khớp!")
        print(f"File 1 có {total_rows_1} dòng, File 2 có {total_rows_2} dòng.")
        print(f"=> LabVIEW đã làm rớt {abs(total_rows_1 - total_rows_2)} bản tin.\n")
    else:
        print(f"[OK] Số lượng dòng khớp nhau: {total_rows_1} dòng.\n")

    # 2. So sánh chi tiết từng dòng
    min_rows = min(total_rows_1, total_rows_2)
    mismatches = []
    
    for i in range(min_rows):
        row1 = data1[i]
        row2 = data2[i]
        
        # Xóa khoảng trắng thừa (nếu có) trước khi so sánh
        row1_clean = [str(item).strip() for item in row1]
        row2_clean = [str(item).strip() for item in row2]

        if row1_clean != row2_clean:
            mismatches.append({
                'row_index': i + 1, # Cộng 1 để giống số dòng trên Excel/Text Editor
                'expected': row1_clean,
                'received': row2_clean
            })

    # 3. Tổng hợp kết quả
    if not mismatches and total_rows_1 == total_rows_2:
        print("🎉 TUYỆT VỜI! Hai file giống nhau 100%. Không có lỗi truyền nhận.")
    else:
        print(f"❌ Phát hiện {len(mismatches)} dòng có dữ liệu bị sai lệch.")
        if mismatches:
            print(f"Hiển thị chi tiết (tối đa 10 dòng lỗi đầu tiên):")
            for idx, err in enumerate(mismatches[:10]):
                print(f"  - Dòng {err['row_index']}:")
                print(f"      + Gửi đi : {err['expected']}")
                print(f"      + Nhận về: {err['received']}")
            
            if len(mismatches) > 10:
                print(f"  ... và {len(mismatches) - 10} dòng lỗi khác.")

# --- Thực thi ---
# Thay đổi đường dẫn cho đúng với máy của bạn
file_gui = r"C:\Users\Lyciales\Documents\UniversityDocuments\CapsuleEndoscopeControl\CEC_Serial\.gitignore\test data\test_cases.csv"
file_nhan = r"C:\Users\Lyciales\Documents\UniversityDocuments\CapsuleEndoscopeControl\CEC_Serial\.gitignore\test data\received.csv"

try:
    compare_csv_files(file_gui, file_nhan)
except FileNotFoundError as e:
    print(f"Lỗi: Không tìm thấy file. Hãy kiểm tra lại đường dẫn.\nChi tiết: {e}")