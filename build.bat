@echo off
setlocal
g++ -std=c++17 -Wall -Wextra -Wpedantic -Iinclude src\main.cpp src\monHoc.cpp src\sinhVien.cpp src\lop.cpp src\dangKi.cpp src\lopTinChi.cpp src\nhapLieu.cpp src\duLieu.cpp src\quanLy.cpp -o QuanLySinhVien.exe
if errorlevel 1 exit /b 1
echo Build thanh cong: QuanLySinhVien.exe
