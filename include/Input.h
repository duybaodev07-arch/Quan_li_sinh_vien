#pragma once

// Chuoi rong tra ve false de ket thuc phan nhap ma.
bool nhapMa(char* ma, int kichThuoc);
void nhapMaBatBuoc(char* ma, int kichThuoc);
void chuanHoaMa(char* ma);
void nhapNienKhoa(char* nienKhoa, int kichThuoc);
void nhapChuoi(char* str, int kichThuoc);
void nhapChuoiChoPhepRong(char* str, int kichThuoc);

int nhapSoNguyenKhongAm();
float nhapSoThuc();
float nhapDiem();
bool laSoDienThoaiHopLe(const char* soDT);
void nhapSoDienThoai(char* soDT, int kichThuoc);
int nhapSoNguyenTrongKhoang(int min, int max);
char nhapXacNhan(); // Y/N
void nhapPhai(char* phai, int kichThuoc); // luu "Nam" hoac "Nu"
