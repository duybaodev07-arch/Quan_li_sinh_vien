#pragma once
#include "Lop.h"
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
// Đếm
int demSoSVDangKy(PTRDK head);
// Thêm
void themCuoiDangKy(PTRDK& head, PTRDK newNode);
bool themDangKy(PTRDK& head, const DangKy& dk);
// Xóa
bool xoaDangKy(PTRDK& head, const char* maSV);
void xoaTatCaDangKy(PTRDK& head);
// In
void inDangKy(const DangKy& dk);
