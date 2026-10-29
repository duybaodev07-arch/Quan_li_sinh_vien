#pragma once
#include <fstream>
#include <sstream>
#include <string>
#include "model/MonHoc.h"
#include "model/Lop.h"
#include "model/LopTinChi.h"
using namespace std;

void ghiMonHocNLR(PTRMH root, ofstream& out);
bool luuTatCaDuLieu(PTRMH root, const DanhSachLop& danhSachLop, const DanhSachLopTinChi& danhSachLopTinChi);
int tachChuoi(const string& dong, char kyTuPhanCach, string* cacMuc, int soMucToiDa);
bool docTatCaDuLieu(PTRMH& root, DanhSachLop& danhSachLop, DanhSachLopTinChi& danhSachLopTinChi, string& thongBaoLoi);

