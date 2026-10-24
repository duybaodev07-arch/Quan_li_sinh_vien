#pragma once

#include <string>
#include "model/DangKi.h"
#include "model/Lop.h"
#include "model/MonHoc.h"

struct LopTinChi {
    int maLopTC = 0;
    char maMH[MAX_MAMH] = "";
    char nienKhoa[MAX_NIENKHOA] = "";
    int hocKy = 0;
    int nhom = 0;
    int minSV = 0;
    int maxSV = 0;
    bool huyLop = false;
    PTRDK dssvdk = nullptr;
};
typedef LopTinChi* PTRLTC;

struct DanhSachLopTinChi {
    PTRLTC lopTC[MAX_LOP_TC] = {};
    int soLuong = 0;
};

int timLopTinChiTheoThamSo(const DanhSachLopTinChi& danhSachLopTinChi, const char* nienKhoa, int hocKy, const char* maMH, int nhom);
int timLopTinChiTheoMa(const DanhSachLopTinChi& danhSachLopTinChi, int maLopTC);
bool kiemTraMonHocDaMoLop(const DanhSachLopTinChi& danhSachLopTinChi, const char* maMH);
bool kiemTraSVDaDangKyLTC(const DanhSachLopTinChi& danhSachLopTinChi, const std::string& maSV);
int layMaLopTCKeTiep(const DanhSachLopTinChi& danhSachLopTinChi);
void giaiPhongDanhSachLopTinChi(DanhSachLopTinChi& danhSachLopTinChi);