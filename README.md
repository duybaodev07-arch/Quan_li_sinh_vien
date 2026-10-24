# Quản lí sinh viên hệ tín chỉ

## Cấu trúc mã nguồn
- `data`: các file dữ liệu đầu vào/đầu ra của chương trình.
- `include/model`: cấu trúc dữ liệu và thao tác CTDL dùng chung.
- `include/features`: khai báo các chức năng theo module.
- `include/storage`: khai báo đọc/ghi dữ liệu.
- `include/ui`: giao diện console đa nền tảng.
- `src/features`: phần cài đặt chức năng.
- `src/storage`: phần cài đặt đọc/ghi file.
- `src/main.cpp`: khởi tạo dữ liệu, gọi menu và dọn bộ nhớ.
- `src/ui/Menu.cpp`: vòng lặp menu và điều phối chức năng.

## Build và chạy
- Windows: chạy `run.bat` (cần cài g++ và thêm vào PATH).
- Linux: chạy `chmod +x run.sh` một lần, sau đó `./run.sh` (cần g++).
- Hoặc dùng `make` để build và `make run` để chạy.

Build dùng C++17.



