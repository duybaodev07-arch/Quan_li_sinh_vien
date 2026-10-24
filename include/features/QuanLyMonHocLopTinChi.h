#pragma once
#include "model/MonHoc.h"
#include "model/Lop.h"
#include "model/LopTinChi.h"

void chucNangA_MoLopTinChi(PTRMH cayMonHoc, DanhSachLopTinChi& danhSachLopTinChi, const DanhSachLop& danhSachLop);
void chucNangB_InDSSVDangKyLopTC(PTRMH cayMonHoc, const DanhSachLopTinChi& danhSachLopTinChi, const DanhSachLop& danhSachLop);
void chucNangE_CapNhatMonHoc(PTRMH& cayMonHoc, const DanhSachLop& danhSachLop, const DanhSachLopTinChi& danhSachLopTinChi);
void chucNangF_InDSMonHocTheoTen(PTRMH cayMonHoc);
void chucNangG_DangKyLopTinChi(PTRMH cayMonHoc, DanhSachLop& danhSachLop, DanhSachLopTinChi& danhSachLopTinChi);
void chucNangH_HuyLopTinChi(PTRMH cayMonHoc, const DanhSachLop& danhSachLop, DanhSachLopTinChi& danhSachLopTinChi);

