#pragma once

#include "DangKi.h"
#include "Lop.h"
#include "MonHoc.h"

#define MAX_LOP_TC 10000

struct LopTinChi {
    int maLopTC = 0;
    char maMH[11] = "";
    char nienKhoa[10] = "";
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

// Tạo
PTRLTC taoLopTinChi();

// Tìm kiếm
int timLopTinChi(const DanhSachLopTinChi& dsLopTC, int maLopTC);
int timLopTinChiTheoMonHoc(const DanhSachLopTinChi& dsLopTC, const char* maMH, const char* nienKhoa, int hocKy, int nhom);

// Nhập
void nhapLopTinChi(LopTinChi& lopTC);
// Nhập để tìm lớp tín chỉ
void nhapThongTinTimLopTinChi(char* maMH, char* nienKhoa, int& hocKy, int& nhom);
// Thêm
bool themLopTinChi(DanhSachLopTinChi& dsLopTC, PTRLTC lopTC);
void themLopTinChiTuBanPhim(DanhSachLopTinChi& dsLopTC);
// Hiệu chỉnh
bool hieuChinhLopTinChi(DanhSachLopTinChi& dsLopTC, int maLopTC);

// Xóa, giai phong
bool xoaLopTinChi(DanhSachLopTinChi& dsLopTC, int maLopTC);
void xoaTatCaLopTinChi(DanhSachLopTinChi& dsLopTC);
// Hủy lớp
bool huyLopTinChi(DanhSachLopTinChi& dsLopTC, int maLopTC);
bool tuDongHuyLopKhongDuSinhVien(LopTinChi& lopTC);
void tuDongHuyDanhSachLopKhongDuSinhVien(DanhSachLopTinChi& dsLopTC);
// In
void inLopTinChi(const LopTinChi& lopTC);
void inDanhSachLopTinChi(const DanhSachLopTinChi& dsLopTC);
void inDanhSachSinhVienDangKy(
    DanhSachLop& dsLop,
    const LopTinChi& lopTC
);
// File
bool ghiFileLopTinChi(const DanhSachLopTinChi& dsLopTC);
bool docFileLopTinChi(DanhSachLopTinChi& dsLopTC);
// nhap diem
bool nhapDiem(DanhSachLopTinChi& dsLopTC, DanhSachLop& dsLop);
// xem bang diem
bool xemBangDiemLopTinChi(DanhSachLopTinChi& dsLopTC, DanhSachLop& dsLop);
// Đăng kí lớp tín chỉ
bool dangKyLopTinChi(DanhSachLopTinChi& dsLopTC,
    DanhSachLop& dsLop,
    PTRMH rootMonHoc);
// Hủy đăng kí tín chỉ
bool huyDangKyLopTinChi(
    DanhSachLopTinChi& dsLopTC
);
