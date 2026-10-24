#pragma once

#include <string>
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

Lop taoLop(const std::string& maLop, const std::string& tenLop);
int timLop(const DanhSachLop& danhSachLop, const std::string& maLop);
PTRSV timSinhVienToanTruong(const DanhSachLop& danhSachLop, const std::string& maSV, int* viTriLopTraVe = nullptr);
void giaiPhongDanhSachLop(DanhSachLop& danhSachLop);