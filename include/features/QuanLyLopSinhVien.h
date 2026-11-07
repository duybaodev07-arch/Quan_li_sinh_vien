#pragma once
#include "model/MonHoc.h"
#include "model/Lop.h"
#include "model/LopTinChi.h"
#include "model/LichSuThaoTac.h"

void chucNangC_CapNhatSinhVienLop(PTRMH& root, DanhSachLop& danhSachLop, DanhSachLopTinChi& danhSachLopTinChi, QuanLyLichSu& lichSu);
void chucNangD_InDSSVTheoMaSV(DanhSachLop& danhSachLop);

