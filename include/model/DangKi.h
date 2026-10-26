#pragma once

#include "model/HangSo.h"

struct DangKy {
    char maSV[MAX_MASV] = {};
    float diem = -1.0f;
    bool huyDangKy = false;
};

struct NodeDK {
    DangKy dk;
    NodeDK* next = nullptr;
};
typedef NodeDK* PTRDK;

DangKy taoDangKy(const char* maSV, float diem = -1.0f, bool huyDangKy = false);
bool themDangKy(PTRDK& dauDanhSach, const DangKy& dangKy);
NodeDK* timDangKy(PTRDK dauDanhSach, const char* maSV);
int demSVDangKyHieuLuc(PTRDK dauDanhSach);
int demTongDangKy(PTRDK dauDanhSach);
void giaiPhongDSDK(PTRDK& dauDanhSach);
