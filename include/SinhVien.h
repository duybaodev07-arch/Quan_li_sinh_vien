#pragma once
#include "Const.h"
struct SinhVien {
    char maSV[MAX_MASV];
    char ho[MAX_HO];
    char ten[MAX_TEN];
    char phai[MAX_PHAI];
    char soDT[MAX_SODT];
};

struct NodeSV {
    // dslk don, xếp theo tên
    SinhVien sv;
    NodeSV* next = nullptr;
};

typedef NodeSV* PTRSV;
// Khoi tao root
void khoiTaoDanhSachSinhVien(PTRSV& root);
// ===== NHAP DU LIEU =====
bool nhapSinhVien(SinhVien& sv); // cho nhập liên tục, mã rỗng out trả false
void nhapThongTinHieuChinh(SinhVien& sv); // ko cho nhap mã

// ===== TAO VA TIM KIEM =====
PTRSV taoNodeSinhVien(const SinhVien& sv); // tạo node gán sv
PTRSV timSinhVien(PTRSV head, const char* maSV);
bool daTonTaiSinhVien(PTRSV head, const char* maSV);

// ===== XU LY THU TU =====
bool sinhVienDungTruoc(const SinhVien& a, const SinhVien& b); // dùng cho sort theo tên
void themSinhVienVaoDSSV(PTRSV& head, PTRSV p);

// ===== THEM / XOA / HIEU CHINH =====
void themSinhVien(PTRSV& head);
bool xoaSinhVien(PTRSV& head, const char* maSV);
bool hieuChinhSinhVien(PTRSV& head, const char* maSV);

// ===== IN DANH SACH =====
void inSinhVien(const SinhVien& sv);
void inDanhSachSinhVien(PTRSV head);
void inDanhSachSinhVienTheoMa(PTRSV head);

// ===== SAP XEP THEO MA =====
// copy node qua mang con tro roi sort theo ma
int demSoSinhVien(PTRSV head);
void sapXepTheoMaSinhVien(PTRSV pSV[], int soLuong);

// ===== GIAI PHONG BO NHO =====
void xoaTatCaSinhVien(PTRSV& head);
// doc, ghi sv