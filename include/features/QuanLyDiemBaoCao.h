#pragma once
#include "model/MonHoc.h"
#include "model/Lop.h"
#include "model/LopTinChi.h"

void chucNangI_NhapDiem(PTRMH cayMonHoc, const DanhSachLop& danhSachLop, DanhSachLopTinChi& danhSachLopTinChi);
void chucNangJ_InBangDiemMonHoc(PTRMH cayMonHoc, const DanhSachLop& danhSachLop, const DanhSachLopTinChi& danhSachLopTinChi);
void chucNangK_InDiemTrungBinhKhoaHoc(PTRMH cayMonHoc, DanhSachLop& danhSachLop, const DanhSachLopTinChi& danhSachLopTinChi);
void chucNangL_BangDiemTongKetLop(PTRMH cayMonHoc, DanhSachLop& danhSachLop, const DanhSachLopTinChi& danhSachLopTinChi);

