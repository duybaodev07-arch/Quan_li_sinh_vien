#pragma once

#include "model/HangSo.h"
#include "model/SinhVien.h"

struct Lop {
    char maLop[MAX_MALOP] = {};
    char tenLop[MAX_TENLOP] = {};
    PTRSV dssv = nullptr;
};

struct DanhSachLop {
    Lop lop[MAX_LOP] = {};
    int soLuong = 0;
};

Lop taoLop(const char* maLop, const char* tenLop);
int timLop(const DanhSachLop& danhSachLop, const char* maLop);
PTRSV timSinhVienToanTruong(const DanhSachLop& danhSachLop, const char* maSV, int* viTriLopTraVe = nullptr);
void giaiPhongDanhSachLop(DanhSachLop& danhSachLop);
