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
using PTRDK = NodeDK*;

// TÌM ĐKI, KIỂM TRA TÌNH TRẠNG ĐKI
PTRDK timDangKyTheoMaSV(PTRDK head, const char* maSV);
bool daDangKy(PTRDK head, const char* maSV);

// ĐẾM, THÊM VÀ HỦY ĐKI
int demSoSVDangKy(PTRDK head);
bool themDangKy(PTRDK& head, const DangKy& dk);
bool huyDangKyTheoMaSV(PTRDK head, const char* maSV);

// GIẢI PHÓNG DS ĐKI
void xoaTatCaDangKy(PTRDK& head);
