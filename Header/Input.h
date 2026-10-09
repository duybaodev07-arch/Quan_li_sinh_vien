#pragma once
// Chỉ cho nhập chữ và số, chuẩn hóa in hoa hết
bool nhapMa(char* ma, int kichThuoc);
// cho phép nhập chuỗi kí tự khác rỗng
void nhapChuoi(char* str, int kichThuoc);

void nhapChuoiChoPhepRong(char* str, int kichThuoc);

int nhapSoNguyenKhongAm();

float nhapSoThuc();

float nhapDiem();

bool laSoDienThoaiHopLe(const char* soDT);

void nhapSoDienThoai(char* soDT, int maxLength);

int nhapSoNguyenTrongKhoang(int min, int max);

char nhapXacNhan(); // Y/N