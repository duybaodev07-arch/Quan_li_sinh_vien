#pragma once

#include "MonHoc.h"
#include "SinhVien.h"
#include "Lop.h"
#include "DangKi.h"
#include "LopTinChi.h"
#include "HienThi.h"

void menuMonHoc(PTRMH& root, const DanhSachLopTinChi& dsLopTC);
void menuSinhVien(DanhSachLop& dsLop, int viTri);
void menuLop(DanhSachLop& dsLop);
void menuLopTinChi(
    DanhSachLopTinChi& dsLopTC,
    DanhSachLop& dsLop,
    PTRMH root
);
void menuDangKy(
    DanhSachLopTinChi& dsLopTC,
    DanhSachLop& dsLop,
    PTRMH rootMonHoc
);
void menuChinh(
    PTRMH& rootMonHoc,
    DanhSachLop& dsLop,
    DanhSachLopTinChi& dsLopTC
);
