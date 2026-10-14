#pragma once
#include "Const.h"
struct DangKy {
    char maSV[MAX_MASV];
    float diem;
    bool huyDangKy;
};

struct NodeDK {
    DangKy dk;
    NodeDK* next = nullptr;
};
typedef NodeDK* PTRDK;

// Tạo node
PTRDK taoNodeDangKy(const DangKy& dk);
// Tìm kiếm
PTRDK timDangKyTheoMaSV(PTRDK head, const char* maSV);
// Kiểm tra
bool daDangKy(PTRDK head, const char* maSV);
bool daDangKyConHieuLuc(PTRDK head, const char* maSV);
// Dem chi cac dang ky chua bi huy.
int demSoSVDangKy(PTRDK head);
// Thêm
void themCuoiDangKy(PTRDK& head, PTRDK newNode);
bool themDangKy(PTRDK& head, const DangKy& dk);
// Huy nghiep vu: giu node va danh dau trang thai.
bool huyDangKyTheoMaSV(PTRDK head, const char* maSV);
bool khoiPhucDangKyTheoMaSV(PTRDK head, const char* maSV);
// Xoa vat ly, chi dung khi can don node khoi danh sach.
bool xoaDangKy(PTRDK& head, const char* maSV);
void xoaTatCaDangKy(PTRDK& head);
// In
void inDangKy(const DangKy& dk);
