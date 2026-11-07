#pragma once
#include "model/MonHoc.h"
#include "model/Lop.h"
#include "model/LopTinChi.h"
#include "model/LichSuThaoTac.h"

void chucNangA_MoLopTinChi(PTRMH& root, DanhSachLopTinChi& danhSachLopTinChi, DanhSachLop& danhSachLop, QuanLyLichSu& lichSu);
void chucNangB_InDSSVDangKyLopTC(PTRMH root, const DanhSachLopTinChi& danhSachLopTinChi, const DanhSachLop& danhSachLop);
void chucNangE_CapNhatMonHoc(PTRMH& root, DanhSachLop& danhSachLop, DanhSachLopTinChi& danhSachLopTinChi, QuanLyLichSu& lichSu);
void chucNangF_InDSMonHocTheoTen(PTRMH root);
void chucNangG_DangKyLopTinChi(PTRMH root, DanhSachLop& danhSachLop, DanhSachLopTinChi& danhSachLopTinChi);
void chucNangH_HuyLopTinChi(PTRMH root, const DanhSachLop& danhSachLop, DanhSachLopTinChi& danhSachLopTinChi);

