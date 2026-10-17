#pragma once

#include "Lop.h"
#include "LopTinChi.h"
#include "MonHoc.h"

bool chonMonHocTuDanhSach(PTRMH root, char* maMH);
bool chonSinhVienTuDanhSach(PTRSV dssv, char* maSV);
bool chonLopTuDanhSach(const DanhSachLop& dsLop, char* maLop);
bool chonLopTinChiTuDanhSach(
    const DanhSachLopTinChi& dsLopTC,
    int& maLopTC
);
bool xacNhanConsole(const char* thongBao);
