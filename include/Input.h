#pragma once

#include <string>

// ==========================================
// 1. CAC HAM NHAP CHUOI & QUY CHUAN MA
// ==========================================
void nhapChuoi(char* str, int kichThuoc);
void nhapChuoiChoPhepRong(char* str, int kichThuoc);
bool nhapMa(char* ma, int kichThuoc); // Chuoi rong tra ve false de dung nhap (Dung cho cau c)
void nhapMaBatBuoc(char* ma, int kichThuoc);
void chuanHoaMa(char* ma);
bool laMaHopLe(const std::string& ma);
bool laTenHopLe(const std::string& ten);
void nhapTen(char* str, int kichThuoc); // Cho pheps chu, so, khoang trang va +#-:.(), dùng để nhập tenMH, tenLop
void nhapHoTenSinhVien(char* str, int kichThuoc); // Chi cho phep chu va khoang trang
void nhapNienKhoa(char* nienKhoa, int kichThuoc); // Dinh dang YYYY-YYYY
void nhapPhai(char* phai, int kichThuoc); // Luu "Nam" hoac "Nu"
bool laSoDienThoaiHopLe(const char* soDT);
void nhapSoDienThoai(char* soDT, int kichThuoc);

// ==========================================
// 2. CAC HAM NHAP SO HOC & LUA CHON
// ==========================================
int nhapSoNguyenKhongAm();
int nhapSoNguyenTrongKhoang(int min, int max);
float nhapSoThuc();
float nhapDiem(float minDiem, float maxDiem); // Nhap diem theo thang tuy chinh
char nhapXacNhan(); // Tra ve Y hoac N 
bool nhapTrangThaiHuy(); // Chuyen đổi Y/N thanh true/false cho cau h
