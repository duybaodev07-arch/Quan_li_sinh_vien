#pragma once

#include "DangKi.h"
#include "Lop.h"
#include "MonHoc.h"
struct LopTinChi;
struct DanhSachLopTinChi;

float nhapDiemTrenBang(short x, short y);

void nhapDiemTrenBang(const DanhSachLop& dsLop, PTRDK dssvdk);

void inBangDiemLopTinChi(const DanhSachLop& dsLop, const LopTinChi& lopTC);

void inBangDiemTrungBinh(
    DanhSachLop& dsLop,
    DanhSachLopTinChi& dsLopTC,
    PTRMH root,
    const char* maLop
);

void inBangDiemTongKet(
    Lop& lop,
    DanhSachLopTinChi& dsLopTC
);