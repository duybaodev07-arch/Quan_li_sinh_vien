#pragma once
#include "SinhVien.h"
#include "Const.h"

struct Lop {
    char maLop[MAX_MALOP];
    char tenLop[MAX_TENLOP];
    PTRSV dssv;
};

struct DanhSachLop {
    Lop lop[MAX_LOP];
    int soLuong;
};
// khoi tao ds lop
void khoiTaoDanhSachLop(DanhSachLop& dsLop);
// Nhập
void nhapLop(Lop& lop);
// Tìm kiếm
int timLopTheoMa(const DanhSachLop& dsLop, const char* maLop);
bool daTonTaiLop(const DanhSachLop& dsLop, const char* maLop);
PTRSV timSinhVienTrongDanhSachLop(const DanhSachLop& dsLop, const char* maSV);
// Thêm
bool themLopVaoDanhSachLop(DanhSachLop& dsLop, const Lop& lop); // check lop đầy
bool themLop(DanhSachLop& dsLop, const Lop& lop); // check trùng mã ỏ đây
// Hiệu chỉnh
bool hieuChinhLop(DanhSachLop& dsLop, const char* maLop);
// Xóa
bool xoaLop(DanhSachLop& dsLop, const char* maLop);
void xoaTatCaLop(DanhSachLop& dsLop);
// In
void inLop(const Lop& lop);
void inDanhSachLop(const DanhSachLop& dsLop);
// File
bool ghiFileLop(const DanhSachLop& dsLop);
bool docFileLop(DanhSachLop& dsLop);