#pragma once
#include <fstream>
#include <sstream>
#include "model/CauTrucDuLieu.h"
using namespace std;

void ghiMonHocNLR(PTRMH cay, ofstream& out);
bool luuTatCaDuLieu(PTRMH cayMonHoc, const DSLop& danhSachLop, const DSLopTinChi& danhSachLopTinChi);
int tachChuoi(const string& dong, char kyTuPhanCach, string* cacMuc, int soMucToiDa);
void khoiTaoDuLieuMau(PTRMH& cayMonHoc, DSLop& danhSachLop, DSLopTinChi& danhSachLopTinChi);
void docTatCaDuLieu(PTRMH& cayMonHoc, DSLop& danhSachLop, DSLopTinChi& danhSachLopTinChi);

