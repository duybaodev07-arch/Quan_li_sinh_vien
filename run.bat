@echo off
chcp 65001 >nul
echo Dang bien dich chuong trinh...
g++ -std=c++17 -O2 -Wall -Wextra -Iinclude src/main.cpp src/storage/DocGhiDuLieu.cpp src/ui/GiaoDienConsole.cpp src/ui/Menu.cpp src/features/QuanLyLopSinhVien.cpp src/features/QuanLyMonHocLopTinChi.cpp src/features/QuanLyDiemBaoCao.cpp -o QuanLyDiemSV.exe
if %errorlevel% neq 0 (
  echo Loi bien dich! Vui long kiem tra lai code.
  pause
  exit /b %errorlevel%
)
cls
QuanLyDiemSV.exe

