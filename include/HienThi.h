#pragma once

#include "LopTinChi.h"

void nhapDiemDanhSachDangKy(const DanhSachLop& dsLop, PTRDK dssvdk);
void inBangDiemLopTinChi(
    const DanhSachLop& dsLop,
    const LopTinChi& lopTC,
    PTRMH rootMonHoc
);
void inBangDiemTrungBinh(
    const DanhSachLop& dsLop,
    const DanhSachLopTinChi& dsLopTC,
    PTRMH rootMonHoc,
    const char* maLop
);
void inBangDiemTongKet(
    const Lop& lop,
    const DanhSachLopTinChi& dsLopTC
);


