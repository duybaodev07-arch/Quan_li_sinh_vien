#pragma once
#include "features/HamDungChung.h"

void chucNangA_MoLopTinChi(PTRMH cayMonHoc, DSLopTinChi& danhSachLopTinChi, const DSLop& danhSachLop);
void chucNangB_InDSSVDangKyLopTC(PTRMH cayMonHoc, const DSLopTinChi& danhSachLopTinChi, const DSLop& danhSachLop);
void chucNangE_CapNhatMonHoc(PTRMH& cayMonHoc, const DSLop& danhSachLop, const DSLopTinChi& danhSachLopTinChi);
void chucNangF_InDSMonHocTheoTen(PTRMH cayMonHoc);
void chucNangG_DangKyLopTinChi(PTRMH cayMonHoc, DSLop& danhSachLop, DSLopTinChi& danhSachLopTinChi);
void chucNangH_HuyLopTinChi(PTRMH cayMonHoc, const DSLop& danhSachLop, DSLopTinChi& danhSachLopTinChi);

