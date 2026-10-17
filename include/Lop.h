#pragma once
#include "SinhVien.h"
#include "Const.h"

struct Lop {
    char maLop[MAX_MALOP];
    char tenLop[MAX_TENLOP];
    PTRSV dssv; // trỏ đến đầu dssv(head)
};

struct DanhSachLop {
    Lop lop[MAX_LOP];
    int soLuong;
};
// KHỞI TẠO DSLOP, GÁN CÁC PTRSV TRONG TỪNG LOP = NULL, SO LUONG LOP = 0
void khoiTaoDanhSachLop(DanhSachLop& dsLop);

// NHẬP THÔNG TIN VÀO LỚP
void nhapLop(Lop& lop);

// THÊM / XÓA / HIỆU CHỈNH THÔNG TIN LỚP 
void themLop(DanhSachLop& dsLop); 
void themSinhVienVaoLop(DanhSachLop& ds, int viTri);
bool xoaLop(DanhSachLop& dsLop, const char* maLop);
bool hieuChinhLop(DanhSachLop& dsLop, const char* maLop);

// TÌM LỚP THEO MÃ, KIỂM TRA LỚP ĐÃ TỒN TẠI
int timLopTheoMa(const DanhSachLop& dsLop, const char* maLop);
bool daTonTaiLop(const DanhSachLop& dsLop, const char* maLop);

// TÌM SINH VIÊN TRONG DSLOP
PTRSV timSinhVienTrongDanhSachLop(const DanhSachLop& dsLop, const char* maSV);

// XÓA TẤT CẢ LỚP, CẦN XÁC NHẬN
void xoaTatCaLop(DanhSachLop& dsLop);

// IN THÔNG TIN 1 LỚP VÀ DANH SÁCH LỚP (KO IN SV)
void inLop(const Lop& lop);
void inDanhSachLop(const DanhSachLop& dsLop);

/*
FILE: FORM ĐỌC GHI FILE:
malop|tenLop

masv|ho|ten|phai|sdt
# (kí tự kết thúc 1 lớp)

*/ 
bool ghiFileLop(const DanhSachLop& dsLop);
bool docFileLop(DanhSachLop& dsLop); 