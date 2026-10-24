#pragma once
#include "features/HamDungChung.h"

void chucNangI_NhapDiem(PTRMH cayMonHoc, const DSLop& danhSachLop, DSLopTinChi& danhSachLopTinChi);
void chucNangJ_InBangDiemMonHoc(PTRMH cayMonHoc, const DSLop& danhSachLop, const DSLopTinChi& danhSachLopTinChi);
void chucNangK_InDiemTrungBinhKhoaHoc(PTRMH cayMonHoc, DSLop& danhSachLop, const DSLopTinChi& danhSachLopTinChi);
void chucNangL_BangDiemTongKetLop(PTRMH cayMonHoc, DSLop& danhSachLop, const DSLopTinChi& danhSachLopTinChi);

