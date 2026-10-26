#pragma once
#include "model/MonHoc.h"
#include "model/Lop.h"
#include "model/LopTinChi.h"

void chucNangI_NhapDiem(PTRMH root, const DanhSachLop& danhSachLop, DanhSachLopTinChi& danhSachLopTinChi);
void chucNangJ_InBangDiemMonHoc(PTRMH root, const DanhSachLop& danhSachLop, const DanhSachLopTinChi& danhSachLopTinChi);
void chucNangK_InDiemTrungBinhKhoaHoc(PTRMH root, DanhSachLop& danhSachLop, const DanhSachLopTinChi& danhSachLopTinChi);
void chucNangL_BangDiemTongKetLop(PTRMH root, DanhSachLop& danhSachLop, const DanhSachLopTinChi& danhSachLopTinChi);

