#pragma once

#include "lop.h"
#include "lopTinChi.h"
#include "monHoc.h"
#include <string>
#include <vector>

bool docDuLieu(const std::string& thuMuc, std::vector<MonHoc>& dsMonHoc, DanhSachLop& dsLop, DanhSachLopTinChi& dsLopTC);
bool ghiDuLieu(const std::string& thuMuc, const std::vector<MonHoc>& dsMonHoc, const DanhSachLop& dsLop, const DanhSachLopTinChi& dsLopTC);
