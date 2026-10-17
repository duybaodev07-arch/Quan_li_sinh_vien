#pragma once

#include "Const.h"
#include "DangKi.h"
#include "Lop.h"
#include "MonHoc.h"

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

using PTRLTC = LopTinChi*;

struct DanhSachLopTinChi {
    PTRLTC lopTC[MAX_LOP_TC] = {};
    int soLuong = 0;
};

int timLopTinChiTheoMa(const DanhSachLopTinChi& dsLopTC, int maLopTC);
int timLopTinChiTheoMonHoc(
    const DanhSachLopTinChi& dsLopTC,
    const char* maMH,
    const char* nienKhoa,
    int hocKy,
    int nhom
);

void nhapLopTinChi(LopTinChi& lopTC, PTRMH rootMonHoc);
void nhapThongTinTimLopTinChi(char* maMH, char* nienKhoa, int& hocKy, int& nhom);
bool themLopTinChiVaoDanhSach(
    DanhSachLopTinChi& dsLopTC,
    PTRLTC lopTC,
    PTRMH rootMonHoc
);
void themLopTinChi(DanhSachLopTinChi& dsLopTC, PTRMH rootMonHoc);
bool hieuChinhLopTinChi(
    DanhSachLopTinChi& dsLopTC,
    int maLopTC,
    PTRMH rootMonHoc
);
bool xoaLopTinChi(DanhSachLopTinChi& dsLopTC, int maLopTC);
void xoaTatCaLopTinChi(DanhSachLopTinChi& dsLopTC);
bool huyLopTinChi(DanhSachLopTinChi& dsLopTC, int maLopTC);
bool tuDongHuyLopKhongDuSinhVien(
    LopTinChi& lopTC,
    const char* nienKhoa,
    int hocKy
);
void tuDongHuyDanhSachLopKhongDuSinhVien(
    DanhSachLopTinChi& dsLopTC,
    const char* nienKhoa,
    int hocKy
);

void inLopTinChi(const LopTinChi& lopTC);
void inDanhSachLopTinChi(const DanhSachLopTinChi& dsLopTC);
void inDanhSachSinhVienDangKy(
    const DanhSachLop& dsLop,
    const LopTinChi& lopTC
);

bool ghiFileLopTinChi(const DanhSachLopTinChi& dsLopTC);
bool docFileLopTinChi(DanhSachLopTinChi& dsLopTC, PTRMH rootMonHoc);
bool nhapDiem(DanhSachLopTinChi& dsLopTC, DanhSachLop& dsLop);
bool xemBangDiemLopTinChi(
    DanhSachLopTinChi& dsLopTC,
    const DanhSachLop& dsLop,
    PTRMH rootMonHoc
);
bool dangKyLopTinChi(
    DanhSachLopTinChi& dsLopTC,
    DanhSachLop& dsLop,
    PTRMH rootMonHoc
);
bool huyDangKyLopTinChi(DanhSachLopTinChi& dsLopTC);
