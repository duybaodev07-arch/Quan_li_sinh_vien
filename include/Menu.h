#pragma once

#include "MonHoc.h"
#include "SinhVien.h"
#include "Lop.h"
#include "DangKi.h"
#include "LopTinChi.h"
#include "HienThi.h"
// QUẢN LÍ MÔN HỌC
void menuMonHoc(PTRMH& root);

// QUẢN LÍ SINH VIÊN, TRUYỀN DSLOP, VITRI ĐỂ TÌM MÃ SV TRÙNG KHI THÊM SV MỚI
void menuSinhVien(DanhSachLop& dsLop, int viTri);

// QUẢN LÍ LỚP
void menuLop(DanhSachLop& dsLop);

// QUẢN LÍ LỚP TÍN CHỈ
void menuLopTinChi(
    DanhSachLopTinChi& dsLopTC,
    DanhSachLop& dsLop,
    PTRMH root
);

// MENU ĐĂNG KÝ
void menuDangKy(
    DanhSachLopTinChi& dsLopTC,
    DanhSachLop& dsLop,
    PTRMH rootMonHoc
);

// MENU CHÍNH, MENU CHA
void menuChinh(
    PTRMH& rootMonHoc,
    DanhSachLop& dsLop,
    DanhSachLopTinChi& dsLopTC
);
