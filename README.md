# Quản lí sinh viên hệ tín chỉ

## Cấu trúc mã nguồn
- `data/`: các file dữ liệu đầu vào/đầu ra.
- `include/model/` và `src/model/`: khai báo kiểu dữ liệu và cài đặt thao tác CTDL.
- `include/features/` và `src/features/`: header khai báo API và source cài đặt theo module nghiệp vụ.
- `include/storage/` và `src/storage/`: API và cài đặt đọc/ghi dữ liệu.
- `include/ui/` và `src/ui/`: API và cài đặt giao diện console, menu.
- `src/ui/GiaoDienConsole.cpp`: các hàm giao diện dùng chung; `GiaoDienConsole_NenTang.cpp`: thao tác console riêng cho Windows. Dự án hiện chỉ build backend Windows.
- `src/main.cpp`: khởi tạo, điều phối và giải phóng dữ liệu.

Mỗi module có header `.h` để khai báo và source `.cpp` để cài đặt. Các hàm hỗ trợ giao diện/dữ liệu dùng chung được khai báo trong `include/features/TienIchChung.h` và cài đặt trong `src/features/TienIchChung.cpp`.

## Build và chạy
- **VS Code:** mở thư mục gốc project rồi nhấn `Ctrl+Shift+B` để build. Cần cài MinGW-w64, `g++` và `mingw32-make` trong `PATH`.
- **Windows:** chạy `run.bat` để build và chạy.
- Có thể dùng `mingw32-make` để build, `mingw32-make run` để build và chạy, `mingw32-make clean` để xóa executable.

Project dùng C++17.
