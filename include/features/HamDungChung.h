#pragma once

#include <string>
#include "model/MonHoc.h"
#include "model/Lop.h"
#include "model/LopTinChi.h"

std::string dinhDangDiem(float diem, bool deTrongNeuChuaCo = false);
std::string chonLopNhanh(DanhSachLop& danhSachLop, bool choPhepTaoMoi = false);
std::string chonMonHocNhanh(PTRMH cayMonHoc);
int chonLopTinChiTheoThongTin(PTRMH cayMonHoc, const DanhSachLopTinChi& danhSachLopTinChi, const std::string& tieuDeManHinh);
